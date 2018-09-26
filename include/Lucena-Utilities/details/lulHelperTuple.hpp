/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“details/HelperTuple.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a subset of libc++’s <tuple> implementation, namely some of the
	private bits, used under the University of Illinois/NCSA Open Source
	License. See “license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <limits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy { namespace details {

// __sfinae_ctor_base
template <bool _CanCopy, bool _CanMove>
struct __sfinae_ctor_base {};
template <>
struct __sfinae_ctor_base<false, false> {
  __sfinae_ctor_base() = default;
  __sfinae_ctor_base(__sfinae_ctor_base const&) = delete;
  __sfinae_ctor_base(__sfinae_ctor_base &&) = delete;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base const&) = default;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base&&) = default;
};
template <>
struct __sfinae_ctor_base<true, false> {
  __sfinae_ctor_base() = default;
  __sfinae_ctor_base(__sfinae_ctor_base const&) = default;
  __sfinae_ctor_base(__sfinae_ctor_base &&) = delete;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base const&) = default;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base&&) = default;
};
template <>
struct __sfinae_ctor_base<false, true> {
  __sfinae_ctor_base() = default;
  __sfinae_ctor_base(__sfinae_ctor_base const&) = delete;
  __sfinae_ctor_base(__sfinae_ctor_base &&) = default;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base const&) = default;
  __sfinae_ctor_base& operator=(__sfinae_ctor_base&&) = default;
};


// __sfinae_assign_base
template <bool _CanCopy, bool _CanMove>
struct __sfinae_assign_base {};
template <>
struct __sfinae_assign_base<false, false> {
  __sfinae_assign_base() = default;
  __sfinae_assign_base(__sfinae_assign_base const&) = default;
  __sfinae_assign_base(__sfinae_assign_base &&) = default;
  __sfinae_assign_base& operator=(__sfinae_assign_base const&) = delete;
  __sfinae_assign_base& operator=(__sfinae_assign_base&&) = delete;
};
template <>
struct __sfinae_assign_base<true, false> {
  __sfinae_assign_base() = default;
  __sfinae_assign_base(__sfinae_assign_base const&) = default;
  __sfinae_assign_base(__sfinae_assign_base &&) = default;
  __sfinae_assign_base& operator=(__sfinae_assign_base const&) = default;
  __sfinae_assign_base& operator=(__sfinae_assign_base&&) = delete;
};
template <>
struct __sfinae_assign_base<false, true> {
  __sfinae_assign_base() = default;
  __sfinae_assign_base(__sfinae_assign_base const&) = default;
  __sfinae_assign_base(__sfinae_assign_base &&) = default;
  __sfinae_assign_base& operator=(__sfinae_assign_base const&) = delete;
  __sfinae_assign_base& operator=(__sfinae_assign_base&&) = default;
};


// __check_tuple_constructor_fail
struct LUL_VIS_CLASS __check_tuple_constructor_fail {
    template <class ...>
    static constexpr bool __enable_default() { return false; }
    template <class ...>
    static constexpr bool __enable_explicit() { return false; }
    template <class ...>
    static constexpr bool __enable_implicit() { return false; }
    template <class ...>
    static constexpr bool __enable_assign() { return false; }
};


// __all
template <bool ..._Preds>
struct __all_dummy;

template <bool ..._Pred>
using __all = std::is_same<__all_dummy<_Pred...>, __all_dummy<((void)_Pred, true)...>>;


//	__find_detail
namespace __find_detail {

static constexpr size_t __not_found = std::numeric_limits <size_t>::max();
static constexpr size_t __ambiguous = __not_found - 1;

inline LUL_VIS_INLINE_FUNC
constexpr size_t __find_idx_return(size_t __curr_i, size_t __res, bool __matches) {
    return !__matches ? __res :
        (__res == __not_found ? __curr_i : __ambiguous);
}

template <size_t _Nx>
inline LUL_VIS_INLINE_FUNC
constexpr size_t __find_idx(size_t __i, const bool (&__matches)[_Nx]) {
  return __i == _Nx ? __not_found :
      __find_idx_return(__i, __find_idx(__i + 1, __matches), __matches[__i]);
}

template <class _T1, class ..._Args>
struct __find_exactly_one_checked {
    static constexpr bool __matches[sizeof...(_Args)] = {std::is_same<_T1, _Args>::value...};
    static constexpr size_t value = __find_detail::__find_idx(0, __matches);
    static_assert(value != __not_found, "type not found in type list" );
    static_assert(value != __ambiguous, "type occurs more than once in type list");
};

template <class _T1>
struct __find_exactly_one_checked<_T1> {
    static_assert(!std::is_same<_T1, _T1>::value, "type not in empty type list");
};

template <class _Tp, class... _Types>
inline LUL_VIS_INLINE_FUNC
constexpr size_t __find_index() {
  constexpr bool __matches[] = {std::is_same_v<_Tp, _Types>...};
  size_t __result = __not_found;
  for (size_t __i = 0; __i < sizeof...(_Types); ++__i) {
    if (__matches[__i]) {
      if (__result != __not_found) {
        return __ambiguous;
      }
      __result = __i;
    }
  }
  return __result;
}

template <size_t _Index>
struct __find_unambiguous_index_sfinae_impl
    : std::integral_constant<size_t, _Index> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__not_found> {};

template <>
struct __find_unambiguous_index_sfinae_impl<__ambiguous> {};

template <class _Tp, class... _Types>
struct __find_unambiguous_index_sfinae
    : __find_unambiguous_index_sfinae_impl<__find_index<_Tp, _Types...>()> {};

} // namespace __find_detail

template <typename _T1, typename... _Args>
struct __find_exactly_one_t
    : public __find_detail::__find_exactly_one_checked<_T1, _Args...> {
};

} } // namespace details, namespace stdproxy

LUL_end_v_namespace
