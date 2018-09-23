/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“VariantWrapper.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <variant> (or lulVariant.hpp). This
	placeholder will select the correct header based on availability. The
	wrapper will import top-level symbols, providing implementation-agnostic
	aliases if necessary, mapping them into the nested namespace stdproxy
	within the versioned lul namespace - or unversioned, if so noted - to avoid
	clashes while providing easy searching/renaming when the wrapper is
	eventually mooted.

	Assuming some kind of “variant” support is available, we’ll see the
	following defined:

	From 23.7.2, class template variant:
	blah

	FIXME This header is unimplemented. The only platforms for which we need
	a variant reference implementatiom are Apple’s, prior to operating systems
	not tied to Xcode 10’s SDKs (similar to std::optional and std::any. Since
	none of our in-house projects use <variant> at the the moment, we’ll
	probably not bother with this prior to changing the minimum macOS and iOS
	version requirements for the library to obviate the need for it.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_VARIANT_NAMESPACE
	#error "LUL_TEMP_VARIANT_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_variant
	#if LUL_LIBCPP17_VARIANT
		#include <variant>

		#define LUL_TEMP_VARIANT_NAMESPACE std
	#elif LUL_LIBCPP17_VARIANT_EXP
		#include <experimental/variant>

		#define LUL_TEMP_VARIANT_NAMESPACE std::experimental
	#endif
#endif

#ifdef LUL_TEMP_VARIANT_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

		//	FIXME aliases go here; without the aliases, <variant> must be
		//	accessed through the appropriate standard namespaces instead of
		//	through LUL_::stdproxy, e.g., std::variant or
		//	std::experimental::variant, etc.

	}	//	namespace stdproxy

	LUL_end_v_namespace

	#define LUL_CONFIG_std_variant_supported 1

	#undef LUL_TEMP_VARIANT_NAMESPACE
#else
	//	APIME Pragmatically speaking, the only platforms that hit this branch
	//	are Apple’s - and the only reason they’re here is because Apple’s C++
	//	runtime doesn’t define bad_bad_variant_access_access.what() - so
	//	simply using the proper clang implementation is nearly a drop-in
	//	solution.
	#include <Lucena-Utilities/details/lulVariant.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif

#ifndef LUL_CONFIG_std_variant_supported
	#define LUL_CONFIG_std_variant_supported 0
#endif	//	LUL_CONFIG_std_variant_supported
