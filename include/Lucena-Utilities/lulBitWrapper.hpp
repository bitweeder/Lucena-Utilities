/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“BitWrapper.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <bit> (or lulBit.hpp). This placeholder
	will select the correct header based on availability. The wrapper will
	import top-level symbols, providing implementation-agnostic aliases if
	necessary, mapping them into the nested namespace stdproxy within the
	versioned lul namespace - or unversioned, if so noted - to avoid clashes
	while providing easy searching/renaming when the wrapper is eventually
	mooted.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_BIT_NAMESPACE
	#error "LUL_TEMP_BIT_NAMESPACE should not be defined"
#endif

#if LUL_LIBCPP2A_BIT_CAST
	#include <bit>

	#define LUL_TEMP_BIT_NAMESPACE std
#endif

#ifdef LUL_TEMP_BIT_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	//	APIME We don’t bother to SFINAE-out since we rely on the real
	//	function to do that.
	template <class T, class U>
	LUL_VIS_INLINE inline constexpr T bit_cast (
		const U & v) noexcept
	{
		return LUL_TEMP_BIT_NAMESPACE::bit_cast <T, U> (v);
	}

	}	//	namespace stdproxy

	LUL_end_v_namespace

	#define LUL_CONFIG_std_bit_cast_supported 1

	#undef LUL_TEMP_BIT_NAMESPACE
#else
	#include <Lucena-Utilities/details/lulBit.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif

#ifndef LUL_CONFIG_std_bit_cast_supported
	#define LUL_CONFIG_std_bit_cast_supported 0
#endif	//	LUL_CONFIG_std_bit_cast_supported
