/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“ConcurrecyTypes.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <mutex>
#include <shared_mutex>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Type Definitions
*/

//	These are convenience types intended to save some typing.
#if LUL_CONFIG_use_shared_lock
	using SharedMutex = LUL_std_abi::shared_timed_mutex;
	using SharedLock = LUL_std_abi::shared_lock <SharedMutex>;
	using ExclusiveLock = LUL_std_abi::unique_lock <SharedMutex>;
#else
	//	Fall back to using the basic mutex; note that this will cause problems
	//	if shared_mutex functionality is explicitly used.
	using SharedMutex = LUL_std_abi::mutex;
	using SharedLock = LUL_std_abi::unique_lock <SharedMutex>;
	using ExclusiveLock = LUL_std_abi::unique_lock <SharedMutex>;
#endif	//	LUL_CONFIG_use_shared_lock


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
