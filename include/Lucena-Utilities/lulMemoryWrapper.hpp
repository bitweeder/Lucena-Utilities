/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“MemoryWrapper.hpp”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <experimental/memory> or library feature
	reference implementation headers such as lulObserverPtr.hpp directly. This
	placeholder will select the correct header based on availability.
	Additionally, this will always include <memory>, and so it can safely be
	used in place of that header; this could act as additional documentation,
	of a sort.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <memory>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


#ifdef LUL_TEMP_INCLUDE_EXPERIMENTAL_MEMORY
	#error "LUL_TEMP_INCLUDE_EXPERIMENTAL_MEMORY should not be defined"
#endif

#ifdef LUL_TEMP_OBSERVER_PTR_NAMESPACE
	#error "LUL_TEMP_OBSERVER_PTR_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_observer_ptr
	#if LUL_LIBCPPTS_OBSERVER_PTR
		#define LUL_TEMP_ANY_NAMESPACE LUL_std_abi
	#elif LUL_LIBCPPTS_OBSERVER_PTR_EXP
		#define LUL_TEMP_ANY_NAMESPACE LUL_std_abi::experimental
		#define LUL_TEMP_INCLUDE_EXPERIMENTAL_MEMORY 1
	#endif
#endif

//	SEEME Arguably, this extra checking is gratuitous in the face of include
//	guards and #pragma once.
#if LUL_TEMP_INCLUDE_EXPERIMENTAL_MEMORY
	#include <experimental/memory>
	#undef LUL_TEMP_INCLUDE_EXPERIMENTAL_MEMORY
#endif


/*------------------------------------------------------------------------------
	Assuming some kind of “observer_ptr” support is available, we’ll see the
	following defined:

	* LUL_::stdproxy::ObserverPtr, as a proxy for std::observer_ptr (or
		equivalent)
	* std::hash specialization on the LUL_::stdproxy::ObserverPtr type
*/

#ifdef LUL_TEMP_OBSERVER_PTR_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	template <class T>
	using observer_ptr = LUL_TEMP_OBSERVER_PTR_NAMESPACE::observer_ptr <T>;

	}	//	namespace stdproxy

	LUL_end_v_namespace
#else
	#include <Lucena-Utilities/details/lulObserverPtr.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif	//	LUL_TEMP_OBSERVER_PTR_NAMESPACE
