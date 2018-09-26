/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Visibility.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This header sets up symbol visibility decorators specific to LUL.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>


/*------------------------------------------------------------------------------
	Visibility Macros

	In order for this to work correctly, LUL_CONFIG_utility_lib must be set to
	a non-zero value when building the library itself, as opposed to just
	linking to it (or including its headers).
*/

#if LUL_CONFIG_utility_lib
	#define LUL_VIS_CLASS							LUL_VIS_CLASS_EXPORT
	#define LUL_VIS_EXTERN_CLASS_TEMPLATE			LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION	LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_EXPORT
	#define LUL_VIS_FUNC							LUL_VIS_FUNC_EXPORT
	#define LUL_VIS_OVERLOADABLE_FUNC				LUL_VIS_OVERLOADABLE_FUNC_EXPORT
	#define LUL_VIS_EXCEPTION						LUL_VIS_EXCEPTION_EXPORT
	#define LUL_VIS_EXTERN							LUL_VIS_EXTERN_EXPORT
#else
	#define LUL_VIS_CLASS							LUL_VIS_CLASS_IMPORT
	#define LUL_VIS_EXTERN_CLASS_TEMPLATE			LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION	LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_IMPORT
	#define LUL_VIS_FUNC							LUL_VIS_FUNC_IMPORT
	#define LUL_VIS_OVERLOADABLE_FUNC				LUL_VIS_OVERLOADABLE_FUNC_IMPORT
	#define LUL_VIS_EXCEPTION						LUL_VIS_EXCEPTION_IMPORT
	#define LUL_VIS_EXTERN							LUL_VIS_EXTERN_IMPORT
#endif	//	LUL_CONFIG_utility_lib
