/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Bit.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <cstring>
#include <limits>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


//	system
#if LUL_TARGET_COMPILER_MSVC && LUL_TARGET_STANDARD_LIBRARY_MSVC
	#include <intrin.h>
#endif


LUL_begin_v_namespace

namespace stdproxy {

namespace details {

#ifdef LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS
	#error "LUL_FEATURE_CONSTEXPR_INTRINSICS should not propagate"
#endif

#ifdef LUL_INTRINSIC_HAS_CLZ
	#error "LUL_INTRINSIC_HAS_CLZ should not propagate"
#endif

#ifdef LUL_INTRINSIC_HAS_POPCNT
	#error "LUL_INTRINSIC_HAS_POPCNT should not propagate"
#endif

#if LUL_FEATURE_CONSTEXPR_INTRINSICS
	#define LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS constexpr
#else
	#define LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS
#endif

/*
	The following pieces related to intrinsic wrappers are derived from the
	libc++ 8.0 <bit> implementation, used under the University of
	Illinois/NCSA Open Source License. See “license/libc++ License” for
	details.

	The notable changes from the original libc++ pieces include:

	* namespace changes
	* renaming of various macros to use LUL versions
	* renaming of reserved symbols
*/

#if LUL_TARGET_COMPILER_MSVC && LUL_TARGET_STANDARD_LIBRARY_MSVC
	//	FIXME _BitScanReverse maps directly to the `bsr` x86 opcode (or
	//	equivalent); for some uses, it would be more efficient to just return
	//	the bsr result rather than translate it to a leading zero count.
	//	Precondition:  v != 0
	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int clz(unsigned v)
	{
		static_assert (sizeof (unsigned) == sizeof (unsigned long));
		static_assert (sizeof (unsigned long) == 4);

		unsigned long w;

		if (_BitScanReverse (&w, v))
		{
			return static_cast <int> (31 - w);
		}

		return 32;		//	Undefined Behavior.
	}

	//	Precondition:  v != 0
	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int clz (unsigned long v)
	{
		static_assert (sizeof (unsigned) == sizeof (unsigned long));

		return clz (static_cast <unsigned> (v));
	}

	//	Precondition:  v != 0
	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int clz (unsigned long long v)
	{
		unsigned long w;

		#if LUL_TARGET_API_WIN64
			if (_BitScanReverse64 (&w, v))
			{
				return static_cast <int> (63 - w);
			}
		#else
			//	Win32 doesn’t have _BitScanForward64 so emulate it with two
			//	32-bit calls.
			if (_BitScanReverse (&w, static_cast <unsigned long> (v >> 32)))
			{
				return static_cast <int> (63 - (w + 32));
			}

			if (_BitScanReverse (&w, static_cast <unsigned long> (v)))
			{
				return static_cast <int> (63 - w);
			}
		#endif

		return 64;		//	Undefined Behavior.
	}

	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int popcount (unsigned v)
	{
		static_assert (sizeof (unsigned) == 4);

		return __popcnt (v);
	}

	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int popcount (unsigned long v)
	{
		static_assert (sizeof (unsigned long) == 4);

		return __popcnt (v);
	}

	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int popcount (unsigned long long v)
	{
		static_assert (sizeof (unsigned long long) == 8);

		return __popcnt64 (v);
	}

	#define LUL_INTRINSIC_HAS_CLZ 1
	#define LUL_INTRINSIC_HAS_POPCNT 1
#elif LUL_TARGET_COMPILER_CLANG || LUL_TARGET_COMPILER_GCC
	//	Count leading zeroes
	template <typename T,
		typename = std::enable_if_t <std::is_integral_v <T>>,
		typename = std::enable_if_t <std::is_unsigned_v <T>>>
	//requires
	//	std::is_integral_v <T> &&
	//	std::is_unsigned_v <T>
	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int clz (T v)
	{
		return __builtin_clz(v);
	}

	//	Count 1-bits in value
	template <typename T,
		typename = std::enable_if_t <std::is_integral_v <T>>,
		typename = std::enable_if_t <std::is_unsigned_v <T>>>
	//requires
	//	std::is_integral_v <T> &&
	//	std::is_unsigned_v <T>
	inline LUL_VIS_INLINE_FUNC
	LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS int popcount (T v)
	{
		return __builtin_popcount(v);
	}

	#define LUL_INTRINSIC_HAS_CLZ 1
	#define LUL_INTRINSIC_HAS_POPCNT 1
#endif

}	//	namespace details


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
LUL_VIS_INLINE_FUNC inline
constexpr To bit_cast (const From& from) noexcept
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
inline LUL_VIS_INLINE_FUNC
constexpr bool ispow2 (T x) noexcept
{
	#if LUL_FEATURE_CONSTEXPR_INTRINSICS && LUL_INTRINSIC_HAS_POPCNT
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
inline LUL_VIS_INLINE_FUNC
constexpr T ceil2 (T x) noexcept
{
	#if LUL_FEATURE_CONSTEXPR_INTRINSICS && LUL_INTRINSIC_HAS_CLZ
		//	SEEME `clz(0)` may fail for some implementations, so we need to
		//	prevent it, hence the trap for both '0' -and- '1'.
		return (x == 0 || x == 1) ? 1
			: 1 << (std::numeric_limits <T>::digits - clz (x - 1));
	#else
		if (x == 0) return 1;

		--x;

		for (std::size_t i = 1; i < std::numeric_limits <T>::digits; i <<= 2)
		{
			x |= x >> i;
		}

		++x;

		return x;
	#endif
}

//	Returns: If x == 0, 0; otherwise the maximal value y such that ispow2(y) is
//	true and y <= x.
template <class T,
	typename = std::enable_if_t <std::is_integral_v<T>>,
	typename = std::enable_if_t <std::is_unsigned_v<T>>>
//requires
//	std::is_integral_v<T> &&
//	std::is_unsigned_v<T>
inline LUL_VIS_INLINE_FUNC
constexpr T floor2 (T x) noexcept
{
	#if LUL_FEATURE_CONSTEXPR_INTRINSICS && LUL_INTRINSIC_HAS_CLZ
		return (x == 0) ? 0
			: 1 << (std::numeric_limits <T>::digits - clz (x) - 1);
	#else
		if (x == 0) return 0;

		for (std::size_t i = 1; i < std::numeric_limits <T>::digits; i <<= 2)
		{
			x |= x >> i;
		}

		x >>= 1;
		++x;

		return x;
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
inline LUL_VIS_INLINE_FUNC
constexpr T log2p1 (T x) noexcept
{
	#if LUL_FEATURE_CONSTEXPR_INTRINSICS && LUL_INTRINSIC_HAS_CLZ
		return (x == 0) ? 0
			: std::numeric_limits <T>::digits - clz (x) - 1;
	#else
		if (x == 0) return 0;

		T v = 0;

		while (x >>= 1) v++;

		return v;
	#endif
}


#ifdef LUL_CONSTEXPR_IF_CONSTEXPR_INTRINSICS
	#undef LUL_FEATURE_CONSTEXPR_INTRINSICS
#endif

#ifdef LUL_INTRINSIC_HAS_CLZ
	#undef LUL_INTRINSIC_HAS_CLZ
#endif

#ifdef LUL_INTRINSIC_HAS_POPCNT
	#undef LUL_INTRINSIC_HAS_POPCNT
#endif

} // namespace stdproxy

LUL_end_v_namespace
