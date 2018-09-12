/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Config_priv.hpp”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
	//	This file MUST be included first.


//	If LUL_CONFIG_function_lib is defined and set to a non-zero value, it
//	is assumed that the current build target is a version of Lucena Utilities
//	itself.
#ifndef LUL_CONFIG_utility_lib
	#define LUL_CONFIG_utility_lib				1
#endif

#ifndef LUL_CONFIG_use_prefix_std
	#define LUL_CONFIG_use_prefix_std			1
#endif


//	Used to establish visibility of Lucena Utilities symbols in the headers.
#ifndef LUL_CONFIG_visible
	#define LUL_CONFIG_visible					LUL_VIS_DEFINE
#endif	//	LUL_CONFIG_visible

#ifndef LUL_CONFIG_type_visible
	#define LUL_CONFIG_type_visible				LUL_VIS_TYPE_DEFINE
#endif	//	LUL_CONFIG_type_visible

#ifndef LUL_CONFIG_exception_visible
	#define LUL_CONFIG_exception_visible		LUL_VIS_EXCEPTION_DEFINE
#endif	//	LUL_CONFIG_exception_visible
