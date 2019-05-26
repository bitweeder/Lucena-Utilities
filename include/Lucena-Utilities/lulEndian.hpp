/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Endian.hpp”
	Portions Copyright © 2000-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This file provides policy-based templated utilities for converting data
	between Big-Endian and Little-Endian architectures.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Note that as there is no “short” suffix, we use casting to avoid warnings.
	While there’s marginal extra work, it’s free at runtime since this is a
	constexpr.
*/

inline constexpr uint16_t Byte_Swap (uint16_t v)
{
	return	static_cast <uint16_t> (((((uint32_t) (v)) <<  8) & 0x0000FF00U)	|
									((((uint32_t) (v)) >>  8) & 0x000000FFU));
}

inline constexpr uint32_t Byte_Swap (uint32_t v)
{
	return	((v << 24) & 0xFF000000UL)					|
			((v <<  8) & 0x00FF0000UL)					|
			((v >>  8) & 0x0000FF00UL)					|
			((v >> 24) & 0x000000FFUL);
}

#if LUL_TYPE_HAS_INT64

inline constexpr uint64_t Byte_Swap (uint64_t v)
{
	return	((v << 56) & 0xFF00000000000000ULL)			|
			((v << 40) & 0x00FF000000000000ULL)			|
			((v << 24) & 0x0000FF0000000000ULL)			|
			((v <<  8) & 0x000000FF00000000ULL)			|
			((v >>  8) & 0x00000000FF000000ULL)			|
			((v >> 24) & 0x0000000000FF0000ULL)			|
			((v >> 40) & 0x000000000000FF00ULL)			|
			((v >> 56) & 0x00000000000000FFULL);
}

#endif	//	LUL_TYPE_HAS_INT64


/*------------------------------------------------------------------------------
	Helper template family which allows a given type and a compatible unsigned
	type to be used interchangeably without having to risk implementation-
	defined behavior when casting.
*/

struct PBigEndian
{
	template <typename T>
	static inline constexpr T BE (T v)
	{
		return v;
	}

	template <typename T>
	static inline constexpr T LE (T v)
	{
		return Byte_Swap (v);
	}

	template <typename T>
	static inline constexpr T NE (T v)
	{
		#if LUL_TARGET_RT_BIG_ENDIAN
			return v;
		#else
			return Byte_Swap (v);
		#endif
	}
};

struct PLittleEndian
{
	template <typename T>
	static inline constexpr T BE (T v)
	{
		return Byte_Swap (v);
	}

	template <typename T>
	static inline constexpr T LE (T v)
	{
		return v;
	}

	template <typename T>
	static inline constexpr T NE (T v)
	{
		#if LUL_TARGET_RT_BIG_ENDIAN
			return Byte_Swap (v);
		#else
			return v;
		#endif
	}
};

struct PNativeEndian
{
	template <typename T>
	static inline constexpr T BE (T v)
	{
		#if LUL_TARGET_RT_BIG_ENDIAN
			return v;
		#else
			return Byte_Swap (v);
		#endif
	}

	template <typename T>
	static inline constexpr T LE (T v)
	{
		#if LUL_TARGET_RT_BIG_ENDIAN
			return Byte_Swap (v);
		#else
			return v;
		#endif
	}

	template <typename T>
	static inline constexpr T NE (T v)
	{
		return v;
	}
};

template <typename T, typename Unsigned, typename Tag,
	typename = std::enable_if_t <std::conjunction_v <
		std::is_integral <Unsigned>,
		std::is_unsigned <Unsigned>,
		std::bool_constant <sizeof (T) == sizeof (Unsigned)>,
		std::negation <std::is_same <T, Unsigned>>>>>
union TEndian
{
	using elem_t = T;
	using unsigned_t = Unsigned;
	using tag_t = Tag;

	elem_t _v;
	unsigned_t _u;

	constexpr explicit TEndian (elem_t v)
		:	_v {v}
	{ }

	constexpr explicit TEndian (unsigned_t v)
		:	_u {v}
	{ }

	inline constexpr elem_t BE()
	{
		return (TEndian <elem_t, unsigned_t, tag_t> (tag_t::BE (_u)))._v;
	}

	inline constexpr elem_t LE()
	{
		return (TEndian <elem_t, unsigned_t, tag_t> (tag_t::LE (_u)))._v;
	}

	inline constexpr elem_t NE()
	{
		return (TEndian <elem_t, unsigned_t, tag_t> (tag_t::NE (_u)))._v;
	}
};

template <typename T, typename Tag,
	typename = std::enable_if_t <std::conjunction_v <
		std::is_integral <T>,
		std::is_unsigned <T>>>>
struct TEndianU
{
	using elem_t = T;
	using tag_t = Tag;

	elem_t _v;

	constexpr explicit TEndianU (elem_t v)
		:	_v {v}
	{ }

	inline constexpr elem_t BE()
	{
		return tag_t::BE (_v);
	}

	inline constexpr elem_t LE()
	{
		return tag_t::LE (_v);
	}

	inline constexpr elem_t NE()
	{
		return tag_t::NE (_v);
	}
};


#if LUL_TYPE_HAS_INT64
	using BEint64_t = TEndian <int64_t, uint64_t, PBigEndian>;
	using BEuint64_t = TEndianU <uint64_t, PBigEndian>;
	using LEint64_t = TEndian <int64_t, uint64_t, PLittleEndian>;
	using LEuint64_t = TEndianU <uint64_t, PLittleEndian>;
	using NEint64_t = TEndian <int64_t, uint64_t, PNativeEndian>;
	using NEuint64_t = TEndianU <uint64_t, PNativeEndian>;
#endif	//	LUL_TYPE_HAS_INT64

using BEint32_t = TEndian <int32_t, uint32_t, PBigEndian>;
using BEuint32_t = TEndianU <uint32_t, PBigEndian>;
using LEint32_t = TEndian <int32_t, uint32_t, PLittleEndian>;
using LEuint32_t = TEndianU <uint32_t, PLittleEndian>;
using NEint32_t = TEndian <int32_t, uint32_t, PNativeEndian>;
using NEuint32_t = TEndianU <uint32_t, PNativeEndian>;

using BEint16_t = TEndian <int16_t, uint16_t, PBigEndian>;
using BEuint16_t = TEndianU <uint16_t, PBigEndian>;
using LEint16_t = TEndian <int16_t, uint16_t, PLittleEndian>;
using LEuint16_t = TEndianU <uint16_t, PLittleEndian>;
using NEint16_t = TEndian <int16_t, uint16_t, PNativeEndian>;
using NEuint16_t = TEndianU <uint16_t, PNativeEndian>;


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
