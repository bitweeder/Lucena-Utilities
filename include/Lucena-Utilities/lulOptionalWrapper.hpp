/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“OptionalWrapper.hpp”
	Copyright © 2014-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <optional> (or lulOptional.hpp). This
	placeholder will select the correct header based on availability. The
	wrapper will import top-level symbols, providing implementation-agnostic
	aliases if necessary, mapping them into the nested namespace stdproxy
	within the versioned lul namespace - or unversioned, if so noted - to avoid
	clashes while providing easy searching/renaming when the wrapper is
	eventually mooted.

	Assuming some kind of “optional” support is available, we’ll see the
	following defined:

	From 23.6.3:
	LUL_v_::stdproxy::optional, aliasing std::optional (or equivalent)

	From 23.6.4:
	LUL_v_::stdproxy::nullopt_t, aliasing std::nullopt_t (or equivalent)
	LUL_v_::stdproxy::nullopt, aliasing std::nullopt (or equivalent)

	From 23.6.5:
	LUL_v_::stdproxy::bad_optional_access, aliasing std::bad_optional_access

	From 23.6.6:
	relational operators comparing 2 optionals
		SEEME ADL should make it generally unnecessary to provide these,
		however we do so for completeness in case the user needs to refer to
		a given operator explicitly.
		APIME The lack of function aliases in C++ necessitates the creation
		of passthrough functions to get the equivalent functionality. There
		could be issues with this approach, but I don’t currently have any
		good alternative ideas; testing shows no problems.

	From 23.6.7:
	comparison with nullopt_t
		SEEME ADL should make it generally unnecessary to provide these,
		however we do so for completeness in case the user needs to refer to
		a given operator explicitly.
		APIME The lack of function aliases in C++ necessitates the creation
		of passthrough functions to get the equivalent functionality. There
		could be issues with this approach, but I don’t currently have any
		good alternative ideas; testing shows no problems.

	From 23.6.8:
	comparison with T
		SEEME ADL should make it generally unnecessary to provide these,
		however we do so for completeness in case the user needs to refer to
		a given operator explicitly.
		APIME The lack of function aliases in C++ necessitates the creation
		of passthrough functions to get the equivalent functionality. There
		could be issues with this approach, but I don’t currently have any
		good alternative ideas; testing shows no problems.

	From 23.6.9:
	LUL_v_::stdproxy::swap, aliasing std::swap
	LUL_v_::stdproxy::make_optional, aliasing std::make_optional
		SEEME Once again, we choose not to rely solely on ADL and lament the
		lack of support for function template aliases.

	From 23.6.10:
	hash support
		Nothing; we rely on the std header to set these up correctly

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_OPTIONAL_NAMESPACE
	#error "LUL_TEMP_OPTIONAL_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_optional
	#if LUL_LIBCPP17_OPTIONAL
		#include <optional>

		#define LUL_TEMP_OPTIONAL_NAMESPACE std
	#elif LUL_LIBCPP17_OPTIONAL_EXP
		#include <experimental/optional>

		#define LUL_TEMP_OPTIONAL_NAMESPACE std::experimental
	#endif
#endif

#ifdef LUL_TEMP_OPTIONAL_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	//	23.6.3:
	template <class T>
	using optional = LUL_TEMP_OPTIONAL_NAMESPACE::optional <T>;


	//	23.6.4:
	using nullopt_t = LUL_TEMP_OPTIONAL_NAMESPACE::nullopt_t;
	using LUL_TEMP_OPTIONAL_NAMESPACE::nullopt;


	//	23.6.5:
	using bad_optional_access =
		LUL_TEMP_OPTIONAL_NAMESPACE::bad_optional_access;


	//	23.6.6:
	//	SEEME We don’t perfect-forward these in order to avoid ambiguity
	//	due to symmetry when resolving functions.
	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator == (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator == <T,U> (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator != (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator != <T,U> (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator < (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator < <T,U> (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator > (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator > <T,U> (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator <= <T,U> (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		const optional<T> & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator >= <T,U> (lhs, rhs);
	}


	//	23.6.7:
	//	APIME Note that there’s no reason to perfect forward these since
	//	nullopt comparisons are always noexcept while T comparisons are
	//	not; the extra genericity doesn’t save us any work.
	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator == (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator == (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator == (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator == (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator != (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator != (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator != (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator != (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator < (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator < (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator < (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator < (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator <= (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator <= (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator > (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator > (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator > (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator > (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		const optional<T> & lhs,
		nullupt_t rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator >= (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		nullupt_t lhs,
		const optional<U> & rhs) noexcept
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator >= (lhs, rhs);
	}


	//	23.6.8:
	//	APIME Note that there’s no reason to perfect forward these since
	//	nullopt comparisons are always noexcept while T comparisons are
	//	not; the extra genericity doesn’t save us any work.
	//	APIME We don’t bother to SFINAE out many of these since we rely on
	//	the real functions to do that.
	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator == (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator == (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator == (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator == (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator != (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator != (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator != (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator != (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator < (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator < (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator < (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator < (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator <= (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator <= (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator > (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator > (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator > (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator > (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		const optional<T> & lhs,
		const U & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator >= (lhs, rhs);
	}

	template <class T, class U>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		const T & lhs,
		const optional<U> & rhs)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::operator >= (lhs, rhs);
	}


	//	23.6.10:
	//	APIME Simple passthroughs. We don’t bother to SFINAE out any of
	//	these since we rely on the real functions to do that.
	template <class T>
	LUL_VIS_INLINE inline void swap (
		optional<T> & lhs,
		optional<T> & rhs) noexcept (
			noexcept (LUL_TEMP_OPTIONAL_NAMESPACE::swap (lhs, rhs)))
	{
		LUL_TEMP_OPTIONAL_NAMESPACE::swap (lhs, rhs);
	}

	template <class T>
	LUL_VIS_INLINE inline constexpr auto make_optional (
		T && v)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::make_optional (
			std::forward <T> (v));
	}

	template <class T, class... Args>
	LUL_VIS_INLINE inline constexpr auto make_optional (
		Args &&... args)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::make_optional <T, Args> (
			std::forward <Args> (args)...);
	}

	template <class T, class U, class... Args>
	LUL_VIS_INLINE inline constexpr auto make_optional (
		std::initializer_list <U> il, Args &&... args)
	{
		return LUL_TEMP_OPTIONAL_NAMESPACE::make_optional <T, U, Args> (
			std::move (il), std::forward <Args> (args)...);
	}

	}	//	namespace stdproxy

	LUL_end_v_namespace

	#define LUL_CONFIG_std_optional_supported 1

	#undef LUL_TEMP_OPTIONAL_NAMESPACE
#else
	//	APIME We no longer use Andrej Krzemienski’s version of this as it’s no
	//	longer quite in sync with where the standard went. Instead, we just use
	//	the libc++ version. Pragmatically speaking, the only platforms that hit
	//	this branch are Apple’s - and the only reason they’re here is because
	//	Apple’s C++ runtime prior to macOS 10.14 and equivalent OSs doesn’t
	//	define bad_optional_access.what() - so simply using the proper clang
	//	implementation is nearly a drop-in solution.
	#include <Lucena-Utilities/details/lulOptional.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif

#ifndef LUL_CONFIG_std_optional_supported
	#define LUL_CONFIG_std_optional_supported 0
#endif	//	LUL_CONFIG_std_optional_supported
