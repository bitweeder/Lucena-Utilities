/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“TypeTraitsWrapper.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <experimental/type_traits> or library
	feature reference implementation headers pertaining to type traits. This
	placeholder will select the correct header based on availability.
	Additionally, this will always include <type_traits>, and so it can safely
	be used in place of that header; this could act as additional
	documentation, of a sort.

	SEEME Note that the latter category of “library feature reference
	implementation headers” does NOT include lulTypeTraits.hpp, which is a
	collection of type traits associated with LUL, rather than the Standard
	Library. Arguably, the naming is confusing.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_INCLUDE_EXPERIMENTAL_TYPE_TRAITS
	#error "LUL_TEMP_INCLUDE_EXPERIMENTAL_TYPE_TRAITS should not be defined"
#endif

#ifdef LUL_TEMP_REMOVE_CVREF_NAMESPACE
	#error "LUL_TEMP_REMOVE_CVREF_NAMESPACE should not be defined"
#endif

#if LUL_LIBCPP2A_STD_REMOVE_CVREF
	#define LUL_TEMP_REMOVE_CVREF_NAMESPACE LUL_std_abi
#endif

//	SEEME Arguably, this extra checking is gratuitous in the face of include
//	guards and #pragma once.
#if LUL_TEMP_INCLUDE_EXPERIMENTAL_TYPE_TRAITS
	#include <experimental/type_traits>
	#undef LUL_TEMP_INCLUDE_EXPERIMENTAL_TYPE_TRAITS
#endif


/*------------------------------------------------------------------------------
	Set up the following aliases (or define the types if they are not already
	defined):

	* LUL_::stdproxy::remove_cvref, as a proxy for std::remove_cvref
*/

#ifdef LUL_TEMP_REMOVE_CVREF_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	template <class T>
	using remove_cvref = LUL_TEMP_OBSERVER_PTR_NAMESPACE::remove_cvref <T>;

	template <typename T>
	using remove_cvref_t = typename remove_cvref <T>::type;

	}	//	namespace stdproxy

	LUL_end_v_namespace
#else
	LUL_begin_v_namespace

	namespace stdproxy {

	template <typename T>
	struct remove_cvref
		:	public std::remove_cv <std::remove_reference_t <T>>
	{
	};

	template <typename T>
	using remove_cvref_t = typename remove_cvref <T>::type;

	}	//	namespace stdproxy

	LUL_end_v_namespace
#endif	//	LUL_TEMP_REMOVE_CVREF_NAMESPACE
