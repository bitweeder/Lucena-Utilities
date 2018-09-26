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
