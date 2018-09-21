/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“FilesystemWrapper.hpp”
	Copyright © 2014-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <filesystem>, <experimental/filesystem>,
	or the boost filesystem headers directly. This placeholder will select the 
	correct headers based on availability.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>
#include <Lucena-Utilities/lulVersion.hpp>

//	SEEME This is a little hinky since the interfaces between the std and boost
//	implementations differ in a number of places. For this reason, we have to
//	be explicit about which one we’re supporting.
//	FIXME This is missing a bunch of type aliases.
#if defined (LUL_TEMP_FILESYSTEM_NAMESPACE) || defined (LUL_TEMP_ERROR_CODE_NAMESPACE)
	#error "LUL_TEMP_FILESYSTEM_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_filesystem
	#if LUL_LIBCPP17_FILESYSTEM
		#include <filesystem>

		#define LUL_CONFIG_std_filesystem_supported 1

		#define LUL_TEMP_FILESYSTEM_NAMESPACE std
		#define LUL_TEMP_ERROR_CODE_NAMESPACE std
	#elif LUL_LIBCPP17_FILESYSTEM_EXP
		#include <experimental/filesystem>

		#define LUL_CONFIG_std_filesystem_supported 1

		#define LUL_TEMP_FILESYSTEM_NAMESPACE std::experimental
		#define LUL_TEMP_ERROR_CODE_NAMESPACE std
	#endif
#endif

#ifdef LUL_TEMP_FILESYSTEM_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy::filesystem {
//		using path = LUL_TEMP_FILESYSTEM_NAMESPACE::filesystem::path;
		using namespace LUL_TEMP_FILESYSTEM_NAMESPACE::filesystem;
		using LUL_TEMP_ERROR_CODE_NAMESPACE::error_code;
	}	//	namespace stdproxy::filesystem

	LUL_end_v_namespace

	#undef LUL_TEMP_FILESYSTEM_NAMESPACE
	#undef LUL_TEMP_ERROR_CODE_NAMESPACE
#else
	#include <Lucena-Utilities/details/lulFilesystem.hpp>

	LUL_begin_v_namespace

	namespace stdproxy::filesystem {
		//	SEEME Bringing this in is an artifact of when we supported
		//	boost::filesystem as an alternative; this can probably go away.
		using std::error_code;
	}	//	namespace stdproxy::filesystem

	LUL_end_v_namespace

#endif

#ifndef LUL_CONFIG_std_filesystem_supported
	#define LUL_CONFIG_std_filesystem_supported 0
#endif	//	LUL_CONFIG_std_filesystem_supported
