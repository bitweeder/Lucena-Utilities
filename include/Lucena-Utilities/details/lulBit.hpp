/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Bit.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	FIXME This is only a fragment of the proposed <bit> header; amusingly, the
	bulk of the header was created to support a number of bit-related classes
	that had not been formally approved, while this piece got tagged in and
	approved fairly quickly. The rest of <bit> has since been voted in, and now
	needs to be added to this reference implemetation.

	FIXME
	Part of the LLVM Project, under the Apache License v2.0 with LLVM
	Exceptions.
	See https://llvm.org/LICENSE.txt for license information.
	SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <cstring>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


//	system
#if defined(_LIBCPP_COMPILER_MSVC)
#include <intrin.h>
#endif


LUL_begin_v_namespace

#ifndef _LIBCPP_COMPILER_MSVC

inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned __x)           { return __builtin_ctz(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned long __x)      { return __builtin_ctzl(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned long long __x) { return __builtin_ctzll(__x); }


inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned __x)           { return __builtin_clz(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned long __x)      { return __builtin_clzl(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned long long __x) { return __builtin_clzll(__x); }


inline _LIBCPP_INLINE_VISIBILITY
int __popcount(unsigned __x)           { return __builtin_popcount(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __popcount(unsigned long __x)      { return __builtin_popcountl(__x); }

inline _LIBCPP_INLINE_VISIBILITY
int __popcount(unsigned long long __x) { return __builtin_popcountll(__x); }

#else  // _LIBCPP_COMPILER_MSVC

// Precondition:  __x != 0
inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned __x) {
  static_assert(sizeof(unsigned) == sizeof(unsigned long), "");
  static_assert(sizeof(unsigned long) == 4, "");
  unsigned long __where;
  if (_BitScanForward(&__where, __x))
    return static_cast<int>(__where);
  return 32;
}

inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned long __x) {
    static_assert(sizeof(unsigned long) == sizeof(unsigned), "");
    return __ctz(static_cast<unsigned>(__x));
}

inline _LIBCPP_INLINE_VISIBILITY
int __ctz(unsigned long long __x) {
    unsigned long __where;
#if defined(_LIBCPP_HAS_BITSCAN64)
    (defined(_M_AMD64) || defined(__x86_64__))
  if (_BitScanForward64(&__where, __x))
    return static_cast<int>(__where);
#else
  // Win32 doesn't have _BitScanForward64 so emulate it with two 32 bit calls.
  if (_BitScanForward(&__where, static_cast<unsigned long>(__x)))
    return static_cast<int>(__where);
  if (_BitScanForward(&__where, static_cast<unsigned long>(__x >> 32)))
    return static_cast<int>(__where + 32);
#endif
  return 64;
}

// Precondition:  __x != 0
inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned __x) {
  static_assert(sizeof(unsigned) == sizeof(unsigned long), "");
  static_assert(sizeof(unsigned long) == 4, "");
  unsigned long __where;
  if (_BitScanReverse(&__where, __x))
    return static_cast<int>(31 - __where);
  return 32; // Undefined Behavior.
}

inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned long __x) {
    static_assert(sizeof(unsigned) == sizeof(unsigned long), "");
    return __clz(static_cast<unsigned>(__x));
}

inline _LIBCPP_INLINE_VISIBILITY
int __clz(unsigned long long __x) {
  unsigned long __where;
#if defined(_LIBCPP_HAS_BITSCAN64)
  if (_BitScanReverse64(&__where, __x))
    return static_cast<int>(63 - __where);
#else
  // Win32 doesn't have _BitScanReverse64 so emulate it with two 32 bit calls.
  if (_BitScanReverse(&__where, static_cast<unsigned long>(__x >> 32)))
    return static_cast<int>(63 - (__where + 32));
  if (_BitScanReverse(&__where, static_cast<unsigned long>(__x)))
    return static_cast<int>(63 - __where);
#endif
  return 64; // Undefined Behavior.
}

inline _LIBCPP_INLINE_VISIBILITY int __popcount(unsigned __x) {
  static_assert(sizeof(unsigned) == 4, "");
  return __popcnt(__x);
}

inline _LIBCPP_INLINE_VISIBILITY int __popcount(unsigned long __x) {
  static_assert(sizeof(unsigned long) == 4, "");
  return __popcnt(__x);
}

inline _LIBCPP_INLINE_VISIBILITY int __popcount(unsigned long long __x) {
  static_assert(sizeof(unsigned long long) == 8, "");
  return __popcnt64(__x);
}

#endif // _LIBCPP_COMPILER_MSVC

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_BIT


/*------------------------------------------------------------------------------
	This is a gently hacked-up copy of JF Bastien’s proposed std::bit_cast
	implementation, used under the MIT License. See “license/bit_cast License”
	for details.

	The notable differences from the original version include:

	* namespace changes
	* eliminatation of macros
	* minor C++17 updates
	* <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0476r2.html>

	SEEME Once concepts are official and implemented, enable_if_t lines can be
	removed and concept lines updated and uncommented.

	Return an object of type To whose *object representation* is equal to
	the object representation of From. If multiple object representations
	could represent the *value representation* of From, then it is
	unspecified which To value is returned. If no *value representation*
	corresponds to To’s object representation then the returned value is
	unspecified.
*/

template <typename To, typename From,
    typename = std::enable_if_t <sizeof (To) == sizeof (From)>,
    typename = std::enable_if_t <std::is_trivially_copyable_v <To>>,
    typename = std::enable_if_t <std::is_trivially_copyable_v <From>>>
//requires
//	sizeof (To) == sizeof (From) &&
//	std::is_trivially_copyable_v <To> &&
//	std::is_trivially_copyable_v <From>
LUL_VIS_INLINE_FUNC inline constexpr To bit_cast (const From& from) noexcept
{
	/*
		APIME Note that it is illegal in C++17 to simply reinterpret_cast
		from a float (or float *) to an int (or int *), even if the sizes are
		known to be compatible. Similarly, the hoary old float-int union trick
		introduces Undefined Behavior in C++17. Finally, note that the use of
		aligned storage is necessary to minimize the impact of the memcpy.
		SEEME In order to be constexpr, this implementation requires a
		constexpr memcpy, which does not exist as of C++17. If the compiler
		doesn’t care that this function can -never- be constexpr, then we can
		just go ahead and use this implementation.
		SEEME Technically, the best results would be obtained by using compiler
		intrinsics (e.g., llvm’s bitcast opcode) instead of memcpy, but in
		practice all tested compilers not only optimized out the function call,
		but also produced a minimal amount of code this way.
		SEEME The current C++2a draft standard text designates this function
		as conditionally constexpr based on a number of requirements that don’t
		simply self-resolve by the usual constexpr rules; as there is no way
		to explicitly declare something conditionally constexpr, this presents
		a puzzle. The rule in question:
		“This function shall be constexpr if and only if To, From, and the
		types of all subobjects of To and From are types T such that:
			— is_union_v<T> is false;
			— is_pointer_v<T> is false;
			— is_member_pointer_v<T> is false;
			— is_volatile_v<T> is false; and
			— T has no non-static data members of reference type.
		Note that there -had- been a way to do this by relying on a peculiar
		interaction between constexpr functions and noexcept, but that behavior
		was accidentally removed from the official Standard, and then was
		subsequently ruled to remain removed.
	*/
	std::aligned_storage_t <sizeof (To), alignof (To)> storage;

	std::memcpy (&storage, &from, sizeof (To));

	return reinterpret_cast <To &> (storage);
}


/*------------------------------------------------------------------------------
	Integral powers of 2
*/

//	Returns: true if x is an integral power of two; false otherwise.
template <class T,
    typename = std::enable_if_t <std::is_integral_v<T>>,
    typename = std::enable_if_t <std::is_unsigned_v<T>>>
//requires
//	std::is_integral_v<T> &&
//	std::is_unsigned_v<T>
constexpr bool ispow2 (T x) noexcept
{
	//	FIXME Identify the circumstances wherein the first option is both
	//	available and more efficient.
	#if 0
		return popcount(x) == 1;
	#else
		return (x != 0) and (0 == (x & (x - 1)));
	#endif
}

//	Returns: The minimal value y such that ispow2(y) is true and y >= x;
//	if y is not representable as a value of type T, the result is an
//	unspecified value.
template <class T,
    typename = std::enable_if_t <std::is_integral_v<T>>,
    typename = std::enable_if_t <std::is_unsigned_v<T>>>
//requires
//	std::is_integral_v<T> &&
//	std::is_unsigned_v<T>
constexpr T ceil2 (T x) noexcept
{
	#if 0
		return popcount(x) == 1;
	#else
		return (x != 0) and (0 == (x & (x - 1)));
	#endif
}

//	Returns: The minimal value y such that ispow2(y) is true and y >= x;
//	If x == 0, 0; otherwise the maximal value y such that ispow2(y) is true
//	and y <= x.
template <class T,
    typename = std::enable_if_t <std::is_integral_v<T>>,
    typename = std::enable_if_t <std::is_unsigned_v<T>>>
//requires
//	std::is_integral_v<T> &&
//	std::is_unsigned_v<T>
constexpr T floor2 (T x) noexcept
{
	#if 0
		return popcount(x) == 1;
	#else
		return (x != 0) and (0 == (x & (x - 1)));
	#endif
}

//	Returns: If x == 0, 0; otherwise one plus the base-2 logarithm of x, with
//	any fractional part discarded.
template <class T,
    typename = std::enable_if_t <std::is_integral_v<T>>,
    typename = std::enable_if_t <std::is_unsigned_v<T>>>
//requires
//	std::is_integral_v<T> &&
//	std::is_unsigned_v<T>
constexpr T log2p1 (T x) noexcept
{
	#if 0
		return popcount(x) == 1;
	#else
		return (x != 0) and (0 == (x & (x - 1)));
	#endif
}


} // namespace stdproxy

LUL_end_v_namespace
