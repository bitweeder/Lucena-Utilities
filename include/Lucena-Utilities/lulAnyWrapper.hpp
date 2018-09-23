/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“AnyWrapper.hpp”
	Copyright © 2014-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <any>, <experimental/any>, or lulAny.hpp
	headers directly. This placeholder will select the correct header based on
	availability.
	
	Assuming some kind of “any” support is available, we’ll see the following
	proxies defined for the corresponding std:: or equivalent types; these
	should be used in lieu of the normal methods of accessing these types.
	
		- LUL_::stdproxy::any
		- LUL_::stdproxy::bad_any_cast
		- LUL_::stdproxy::any_cast

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_ANY_NAMESPACE
	#error "LUL_TEMP_ANY_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_any
	#if LUL_LIBCPP17_ANY
		#include <any>

		#define LUL_TEMP_ANY_NAMESPACE LUL_std_abi
	#elif LUL_LIBCPP17_ANY_EXP
		#include <experimental/any>

		#define LUL_TEMP_ANY_NAMESPACE LUL_std_abi::experimental
	#endif
#endif

#ifdef LUL_TEMP_ANY_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	using any = LUL_TEMP_ANY_NAMESPACE::any;

	using bad_any_cast = LUL_TEMP_ANY_NAMESPACE::bad_any_cast;

	template <typename T>
	constexpr auto any_cast = LUL_TEMP_ANY_NAMESPACE::any_cast <T>;

	}	//	namespace stdproxy

	LUL_end_v_namespace

	#define LUL_CONFIG_std_any_supported 1

	#undef LUL_TEMP_ANY_NAMESPACE
#else
	#include <Lucena-Utilities/details/lulAny.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif

#ifndef LUL_CONFIG_std_any_supported
	#define LUL_CONFIG_std_any_supported 0
#endif	//	LUL_CONFIG_std_any_supported
