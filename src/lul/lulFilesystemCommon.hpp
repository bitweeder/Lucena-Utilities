/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“FilesystemCommon.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked-up copy of filesystem_common.h taken from the
	libc++ 7 filesystem implementation, and used under the University of
	Illinois/NCSA Open Source License. See “license/libc++ License” for
	details.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only


//	std
#include <array>
#include <chrono>
#include <cstdarg>
#include <cstdlib>
#include <climits>
#include <Lucena-Utilities/details/lulFilesystem.hpp>
#include <limits>
#include <memory>
#include <string>
#include <system_error>
#include <type_traits>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/time.h>
#include <fcntl.h>


//	lul
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include "lulConfig_priv.hpp"


#if !defined (__APPLE__)
	//	We can use the presence of UTIME_OMIT to detect platforms that provide
	//	utimensat.
	#if defined (UTIME_OMIT)
		#define LUL_USE_UTIMENSAT
	#endif
#endif


LUL_begin_v_namespace

namespace stdproxy::filesystem::detail { namespace {

/*------------------------------------------------------------------------------
*/

static std::string format_string_imp(const char* msg, ...) {
  // we might need a second shot at this, so preemptively make a copy
  struct GuardVAList {
    std::va_list& target;
    bool active = true;
    GuardVAList(std::va_list& target) : target(target), active(true) {}
    void clear() {
      if (active)
        va_end(target);
      active = false;
    }
    ~GuardVAList() {
      if (active)
        va_end(target);
    }
  };
  std::va_list args;
  va_start(args, msg);
  GuardVAList args_guard(args);

  std::va_list args_cp;
  va_copy(args_cp, args);
  GuardVAList args_copy_guard(args_cp);

  std::array<char, 256> local_buff;
  size_t size = local_buff.size();
  auto ret = std::vsnprintf(local_buff.data(), size, msg, args_cp);

  args_copy_guard.clear();

  // handle empty expansion
  if (ret == 0)
    return std::string{};
  if (static_cast<size_t>(ret) < size)
    return std::string(local_buff.data());

  // we did not provide a long enough buffer on our first attempt.
  // add 1 to size to account for null-byte in size cast to prevent overflow
  size = static_cast<size_t>(ret) + 1;
  auto buff_ptr = std::unique_ptr<char[]>(new char[size]);
  ret = std::vsnprintf(buff_ptr.get(), size, msg, args);
  return std::string(buff_ptr.get());
}

[[maybe_unused]] const char* unwrap(std::string const& s) { return s.c_str(); }
const char* unwrap(path const& p) { return p.native().c_str(); }
template <class Arg>
Arg const& unwrap(Arg const& a) {
  static_assert(!std::is_class<Arg>::value, "cannot pass class here");
  return a;
}

template <class... Args>
std::string format_string(const char* fmt, Args const&... args) {
  return format_string_imp(fmt, unwrap(args)...);
}

std::error_code capture_errno() {
  // C++2a illegal
//[[assert: errno]];
  return std::error_code(errno, std::generic_category());
}

template <class T>
T error_value();
template <>
constexpr void error_value<void>() {}
template <>
[[maybe_unused]] bool error_value<bool>() {
  return false;
}
template <>
[[maybe_unused]] uintmax_t error_value<uintmax_t>() {
  return uintmax_t(-1);
}
template <>
constexpr file_time_type error_value<file_time_type>() {
  return file_time_type::min();
}
template <>
[[maybe_unused]] path error_value<path>() {
  return {};
}

template <class T>
struct ErrorHandler {
  const char* func_name;
  std::error_code* ec = nullptr;
  const path* p1 = nullptr;
  const path* p2 = nullptr;

  ErrorHandler(const char* fname, std::error_code* ec, const path* p1 = nullptr,
               const path* p2 = nullptr)
      : func_name(fname), ec(ec), p1(p1), p2(p2) {
    if (ec)
      ec->clear();
  }

  T report(const std::error_code& m_ec) const {
    if (ec) {
      *ec = m_ec;
      return error_value<T>();
    }
    std::string what = std::string("in ") + func_name;
    switch (bool(p1) + bool(p2)) {
    case 0:
      __throw_filesystem_error(what, m_ec);
    case 1:
      __throw_filesystem_error(what, *p1, m_ec);
    case 2:
      __throw_filesystem_error(what, *p1, *p2, m_ec);
	default:
      // unreachable
      __throw_filesystem_error(what, m_ec);
    }
  }

  template <class... Args>
  T report(const std::error_code& m_ec, const char* msg, Args const&... args) const {
    if (ec) {
      *ec = m_ec;
      return error_value<T>();
    }
    std::string what =
        std::string("in ") + func_name + ": " + format_string(msg, args...);
    switch (bool(p1) + bool(p2)) {
    case 0:
      __throw_filesystem_error(what, m_ec);
    case 1:
      __throw_filesystem_error(what, *p1, m_ec);
    case 2:
      __throw_filesystem_error(what, *p1, *p2, m_ec);
	default:
      // unreachable
      __throw_filesystem_error(what, m_ec);
    }
  }

  T report(std::errc const& err) const { return report(std::make_error_code(err)); }

  template <class... Args>
  T report(std::errc const& err, const char* msg, Args const&... args) const {
    return report(std::make_error_code(err), msg, args...);
  }

private:
  ErrorHandler(ErrorHandler const&) = delete;
  ErrorHandler& operator=(ErrorHandler const&) = delete;
};

using std::chrono::duration;
using std::chrono::duration_cast;

using TimeSpec = struct ::timespec;
using StatT = struct ::stat;

template <class FileTimeT, class TimeT,
          bool IsFloat = std::is_floating_point<typename FileTimeT::rep>::value>
struct time_util_base {
  using rep = typename FileTimeT::rep;
  using fs_duration = typename FileTimeT::duration;
  using fs_seconds = duration<rep>;
  using fs_nanoseconds = duration<rep, std::nano>;
  using fs_microseconds = duration<rep, std::micro>;

  static constexpr rep max_seconds =
      duration_cast<fs_seconds>(FileTimeT::duration::max()).count();

  static constexpr rep max_nsec =
      duration_cast<fs_nanoseconds>(FileTimeT::duration::max() -
                                    fs_seconds(max_seconds))
          .count();

  static constexpr rep min_seconds =
      duration_cast<fs_seconds>(FileTimeT::duration::min()).count();

  static constexpr rep min_nsec_timespec =
      duration_cast<fs_nanoseconds>(
          (FileTimeT::duration::min() - fs_seconds(min_seconds)) +
          fs_seconds(1))
          .count();

private:
  static constexpr fs_duration get_min_nsecs() {
    return duration_cast<fs_duration>(
        fs_nanoseconds(min_nsec_timespec) -
        duration_cast<fs_nanoseconds>(fs_seconds(1)));
  }
  // Static assert that these values properly round trip.
  static_assert(fs_seconds(min_seconds) + get_min_nsecs() ==
                    FileTimeT::duration::min(),
                "value doesn't roundtrip");

  static constexpr bool check_range() {
    // This kinda sucks, but it's what happens when we don't have __int128_t.
    if (sizeof(TimeT) == sizeof(rep)) {
      typedef duration<long long, std::ratio<3600 * 24 * 365> > Years;
      return duration_cast<Years>(fs_seconds(max_seconds)) > Years(250) &&
             duration_cast<Years>(fs_seconds(min_seconds)) < Years(-250);
    }
    return max_seconds >= std::numeric_limits<TimeT>::max() &&
           min_seconds <= std::numeric_limits<TimeT>::min();
  }
  static_assert(check_range(), "the representable range is unacceptable small");
};

template <class FileTimeT, class TimeT>
struct time_util_base<FileTimeT, TimeT, true> {
  using rep = typename FileTimeT::rep;
  using fs_duration = typename FileTimeT::duration;
  using fs_seconds = duration<rep>;
  using fs_nanoseconds = duration<rep, std::nano>;
  using fs_microseconds = duration<rep, std::micro>;

  static const rep max_seconds;
  static const rep max_nsec;
  static const rep min_seconds;
  static const rep min_nsec_timespec;
};

template <class FileTimeT, class TimeT>
const typename FileTimeT::rep
    time_util_base<FileTimeT, TimeT, true>::max_seconds =
        duration_cast<fs_seconds>(FileTimeT::duration::max()).count();

template <class FileTimeT, class TimeT>
const typename FileTimeT::rep time_util_base<FileTimeT, TimeT, true>::max_nsec =
    duration_cast<fs_nanoseconds>(FileTimeT::duration::max() -
                                  fs_seconds(max_seconds))
        .count();

template <class FileTimeT, class TimeT>
const typename FileTimeT::rep
    time_util_base<FileTimeT, TimeT, true>::min_seconds =
        duration_cast<fs_seconds>(FileTimeT::duration::min()).count();

template <class FileTimeT, class TimeT>
const typename FileTimeT::rep
    time_util_base<FileTimeT, TimeT, true>::min_nsec_timespec =
        duration_cast<fs_nanoseconds>((FileTimeT::duration::min() -
                                       fs_seconds(min_seconds)) +
                                      fs_seconds(1))
            .count();

template <class FileTimeT, class TimeT, class TimeSpecT>
struct time_util : time_util_base<FileTimeT, TimeT> {
  using Base = time_util_base<FileTimeT, TimeT>;
  using Base::max_nsec;
  using Base::max_seconds;
  using Base::min_nsec_timespec;
  using Base::min_seconds;

  using typename Base::fs_duration;
  using typename Base::fs_microseconds;
  using typename Base::fs_nanoseconds;
  using typename Base::fs_seconds;

public:
  template <class CType, class ChronoType>
  static constexpr bool checked_set(CType* out,
                                                        ChronoType time) {
    using Lim = std::numeric_limits<CType>;
    if (time > Lim::max() || time < Lim::min())
      return false;
    *out = static_cast<CType>(time);
    return true;
  }

  static constexpr bool is_representable(TimeSpecT tm) {
    if (tm.tv_sec >= 0) {
      return tm.tv_sec < max_seconds ||
             (tm.tv_sec == max_seconds && tm.tv_nsec <= max_nsec);
    } else if (tm.tv_sec == (min_seconds - 1)) {
      return tm.tv_nsec >= min_nsec_timespec;
    } else {
      return tm.tv_sec >= min_seconds;
    }
  }

  static constexpr bool is_representable(FileTimeT tm) {
    auto secs = duration_cast<fs_seconds>(tm.time_since_epoch());
    auto nsecs = duration_cast<fs_nanoseconds>(tm.time_since_epoch() - secs);
    if (nsecs.count() < 0) {
      secs = secs + fs_seconds(1);
      nsecs = nsecs + fs_seconds(1);
    }
    using TLim = std::numeric_limits<TimeT>;
    if (secs.count() >= 0)
      return secs.count() <= TLim::max();
    return secs.count() >= TLim::min();
  }

  static constexpr FileTimeT
  convert_from_timespec(TimeSpecT tm) {
    if (tm.tv_sec >= 0 || tm.tv_nsec == 0) {
      return FileTimeT(fs_seconds(tm.tv_sec) +
                       duration_cast<fs_duration>(fs_nanoseconds(tm.tv_nsec)));
    } else { // tm.tv_sec < 0
      auto adj_subsec = duration_cast<fs_duration>(fs_seconds(1) -
                                                   fs_nanoseconds(tm.tv_nsec));
      auto Dur = fs_seconds(tm.tv_sec + 1) - adj_subsec;
      return FileTimeT(Dur);
    }
  }

  template <class SubSecT>
  static constexpr bool
  set_times_checked(TimeT* sec_out, SubSecT* subsec_out, FileTimeT tp) {
    auto dur = tp.time_since_epoch();
    auto sec_dur = duration_cast<fs_seconds>(dur);
    auto subsec_dur = duration_cast<fs_nanoseconds>(dur - sec_dur);
    // The tv_nsec and tv_usec fields must not be negative so adjust accordingly
    if (subsec_dur.count() < 0) {
      if (sec_dur.count() > min_seconds) {
        sec_dur = sec_dur - fs_seconds(1);
        subsec_dur = subsec_dur + fs_seconds(1);
      } else {
        subsec_dur = fs_nanoseconds::zero();
      }
    }
    return checked_set(sec_out, sec_dur.count()) &&
           checked_set(subsec_out, subsec_dur.count());
  }
  static constexpr bool convert_to_timespec(TimeSpecT& dest,
                                                                FileTimeT tp) {
    if (!is_representable(tp))
      return false;
    return set_times_checked(&dest.tv_sec, &dest.tv_nsec, tp);
  }
};

using fs_time = time_util<file_time_type, time_t, TimeSpec>;

#if defined(__APPLE__)
[[maybe_unused]] TimeSpec extract_mtime(StatT const& st) { return st.st_mtimespec; }
[[maybe_unused]] TimeSpec extract_atime(StatT const& st) { return st.st_atimespec; }
#else
[[maybe_unused]] TimeSpec extract_mtime(StatT const& st) { return st.st_mtim; }
[[maybe_unused]] TimeSpec extract_atime(StatT const& st) { return st.st_atim; }
#endif

// allow the utimes implementation to compile even it we're not going
// to use it.

bool posix_utimes(const path& p, std::array<TimeSpec, 2> const& TS,
                  std::error_code& ec) {
  using namespace std::chrono;
  auto Convert = [](long nsec) {
    using int_type = decltype(std::declval< ::timeval>().tv_usec);
    auto dur = duration_cast<microseconds>(nanoseconds(nsec)).count();
    return static_cast<int_type>(dur);
  };
  struct ::timeval ConvertedTS[2] = {{TS[0].tv_sec, Convert(TS[0].tv_nsec)},
                                     {TS[1].tv_sec, Convert(TS[1].tv_nsec)}};
  if (::utimes(p.c_str(), ConvertedTS) == -1) {
    ec = capture_errno();
    return true;
  }
  return false;
}

#if defined(LUL_USE_UTIMENSAT)
bool posix_utimensat(const path& p, std::array<TimeSpec, 2> const& TS,
                     error_code& ec) {
  if (::utimensat(AT_FDCWD, p.c_str(), TS.data(), 0) == -1) {
    ec = capture_errno();
    return true;
  }
  return false;
}
#endif

[[maybe_unused]] bool set_file_times(const path& p, std::array<TimeSpec, 2> const& TS,
                    std::error_code& ec) {
#if !defined(LUL_USE_UTIMENSAT)
  return posix_utimes(p, TS, ec);
#else
  return posix_utimensat(p, TS, ec);
#endif
}


/*----------------------------------------------------------------------------*/

} }	//	stdproxy::filesystem::detail

LUL_end_v_namespace

#endif	//	LUL_CONFIG_headers_only
