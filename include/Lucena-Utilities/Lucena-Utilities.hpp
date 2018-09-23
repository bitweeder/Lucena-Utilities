/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Lucena-Utilities.h”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This master header file includes all of the Lucena Utilities public 
	interface headers. It is intended for use as a prefix header, and it is 
	also safe for use in precompiled headers. Most users should simply include 
	this header and not worry about the component headers.

------------------------------------------------------------------------------*/


#pragma once


//	Configure our public and private build options. These must be consistent
//	between an executable and any libraries it uses.
#include <Lucena-Utilities/lulConfig.hpp>


//	Handle feature detection and set up build environment macros.
#include <Lucena-Utilities/lulFeatureSetup.hpp>


//	Used to establish visibility of Lucena Utilities symbols in the headers.
#ifndef LUL_CONFIG_visible
	#define LUL_CONFIG_visible						LUL_VIS_EXTERN
#endif	//	LUL_CONFIG_visible

#ifndef LUL_CONFIG_type_visible
	#define LUL_CONFIG_type_visible					LUL_VIS_TYPE_EXTERN
#endif	//	LUL_CONFIG_type_visible

#ifndef LUL_CONFIG_exception_visible
	#define LUL_CONFIG_exception_visible			LUL_VIS_EXCEPTION_EXTERN
#endif	//	LUL_CONFIG_exception_visible


//	lul
//	Note that these are included in the STL prefix header, but are generally
//	expected to be manually included by the user to indicate a desire to
//	provide a given Standard Library feature that is not necessarily built in.
//	Where - and if - a LUL header includes one of these wrappers, that should
//	be considered an implementation detail.
//#include <Lucena-Utilities/lulAnyWrapper.hpp>
//#include <Lucena-Utilities/lulBitWrapper.hpp>
//#include <Lucena-Utilities/lulFilesystemWrapper.hpp>
//#include <Lucena-Utilities/lulMemoryWrapper.hpp>
//#include <Lucena-Utilities/lulNetworkingWrapper.hpp>
//#include <Lucena-Utilities/lulOptionalWrapper.hpp>
//#include <Lucena-Utilities/lulSpanWrapper.hpp>
//#include <Lucena-Utilities/lulVariantWrapper.hpp>

#include <Lucena-Utilities/lulConcurrencyTypes.hpp>
#include <Lucena-Utilities/lulEndian.hpp>
#include <Lucena-Utilities/lulIterator.hpp>
#include <Lucena-Utilities/lulMemoryTypes.hpp>
#include <Lucena-Utilities/lulPackaging.hpp>
#include <Lucena-Utilities/lulTime.hpp>
#include <Lucena-Utilities/lulTypes.hpp>
#include <Lucena-Utilities/lulTypeTraits.hpp>
#include <Lucena-Utilities/lulUtilities.hpp>
#include <Lucena-Utilities/lulVersion.hpp>


//	prototypes
LUL_begin_v_namespace

#if !LUL_CONFIG_headers_only
	bool Initialize();
	bool Finalize();
#endif

LUL_end_v_namespace
