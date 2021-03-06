/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“PlatformSetup.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	See lulFeatureSetup.hpp for descriptions of the various macros and flags.

------------------------------------------------------------------------------*/


#pragma once


//	std
#if defined (_MSC_VER) && defined (_WIN32)
	//	We guard inclusion of system headers when using MSVS due to its
	//	noisiness at high warning levels.
	#pragma warning (push, 0)
#endif

#if __has_include (<version>)
	#include <version>
		//	Once this header is universally available, it will take the place
		//	of <ciso646> abuse, providing the same functionality as well as
		//	(eventually) acting as a clearinghouse for SD-6 macros related to
		//	library features.
		//	SEEME This must not be lulVersion.hpp, as that header depends on
		//	this one.
#else
	#include <ciso646>
		//	In C++, this is a do-nothing header we include just for the side
		//	effects: the Standard Library implementation will be configured
		//	and many assorted compiler-dependent feature detection macros will
		//	be defined.
#endif

#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


/*------------------------------------------------------------------------------
	Definition Tests

	Note that none of the LUL_XXX macros are set to 0 until after the compiler
	tests, so it’s sufficient to simply test them for existence. Just to be
	sure, though, we make sure someone didn’t define one of these behind our
	backs, with the following exceptions:

		- LUL_TARGET_OS_xxx should be set by the build system or in some
			similar fashion, as otherwise there may be no way to differentiate
			between certain platforms.

		- LUL_TARGET_API_xxx can be set by the build system or in some
			similar fashion to avoid ambiguities (e.g., determine whether we’re
			targeting Carbon or Cocoa APIs deuner older MacOS X’s). It’s not
			generally necessary to do this any more.
*/

#if !defined (LUL_TARGET_OS_X11) && \
	!defined (LUL_TARGET_OS_OSX) && \
	!defined (LUL_TARGET_OS_WIN) && \
	!defined (LUL_TARGET_OS_IOS) && \
	!defined (LUL_TARGET_OS_IOS_SIM)

	#error "A LUL_TARGET_OS_xxx must be defined outside of this file."
#endif	//	LUL_TARGET_OS check

#if defined (LUL_FEATURE_UTF16_FILE_SYSTEM)
	#error "Don’t define LUL_FEATURE_UTF16_FILE_SYSTEM outside of this file."
#endif	//	LUL_FEATURE_UTF16_FILE_SYSTEM


/*------------------------------------------------------------------------------
	Platform Macros

	SEEME Arguably, it would be more desirable to give each platform its own
	#include file and include them here conditionally rather than go the
	monolithic route. We trade off potential unnecessary recompilation of
	unaffected projects for simplified maintenance at this time.
*/

#if (LUL_TARGET_OS_OSX || LUL_TARGET_OS_IOS || LUL_TARGET_OS_IOS_SIM)
	#if __has_include (<Availability.h>)
		//	SEEME If we get here, we assume we’re also using Apple’s Standard
		//	Library runtime; a more precise test may be necessary if this turns
		//	out to be an overly broad assumption.

		#include <Availability.h>
			//	Special Apple header required for compile-time detection of the
			//	SDK version and the targeted platform. Note that there’s no magic
			//	here; historically the values are all dependent on which platform
			//	SDK is being used, so if the user botches that, everything else
			//	will be wrong.

		#if LUL_TARGET_OS_OSX
			#define LUL_TARGET_API_COCOA				1
		#elif LUL_TARGET_OS_IOS || LUL_TARGET_OS_IOS_SIM
			#define LUL_TARGET_API_COCOA_TOUCH			1
		#else
			#error "Unsupported LUL_TARGET_OS_xxx option"
		#endif

		//	Note that although shared_mutex and friends have been declared in the
		//	libc++ headers for a long time, it was many OS releases before Apple
		//	got around to including the necessary object code in their runtimes,
		//	effectively leaving little bombs to surprise the unwary. We warn if the
		//	minimum supprted OS, as specified by the user, is not up to the task of
		//	providing shared locks, but only if the user has indicated the intent
		//	to use them via the relevant config switch.
		#if LUL_CONFIG_use_shared_lock && _LIBCPP_VERSION &&					\
			((defined (__MAC_OS_X_VERSION_MIN_REQUIRED) &&						\
					(__MAC_OS_X_VERSION_MIN_REQUIRED < 101200)) ||				\
				(defined (__IPHONE_OS_VERSION_MIN_REQUIRED) &&					\
					(__IPHONE_OS_VERSION_MIN_REQUIRED < 100000)) ||				\
				(defined (__TV_OS_VERSION_MIN_REQUIRED) &&						\
					(__TV_OS_VERSION_MIN_REQUIRED < 100000)) ||					\
				(defined (__WATCH_OS_VERSION_MIN_REQUIRED) &&					\
					(__WATCH_OS_VERSION_MIN_REQUIRED < 30000)))
			#error "The minimum OS requirement for std::shared_lock has not been met."
		#endif

		//	Until Xcode 10.0, <experimental/any> is provided, but there is no
		//	support for it in the runtime library (in particular, there is no
		//	implementation of bad_any_cast, among other things). Xcode 10+
		//	includes <any>, but the necessary object code is only available under
		//	macOS 10.14+ and iOS 12+ (and equivalent-generation OS’s), so we force
		//	std::any detection to fail if the minimum-targeted OS is not new
		//	enough, otherwise setting LUL_LIBCPP17_ANY in lulAnyWrapper.hpp. Note
		//	that the same situation and solution applies to <optional> and
		//	<variant>, as well.
		#if defined (_LIBCPP_VERSION) &&										\
			((_LIBCPP_VERSION < 6000) ||										\
			((defined (__MAC_OS_X_VERSION_MIN_REQUIRED) &&						\
					(__MAC_OS_X_VERSION_MIN_REQUIRED < 101400)) ||				\
				(defined (__IPHONE_OS_VERSION_MIN_REQUIRED) &&					\
					(__IPHONE_OS_VERSION_MIN_REQUIRED < 120000)) ||				\
				(defined (__TV_OS_VERSION_MIN_REQUIRED) &&						\
					(__TV_OS_VERSION_MIN_REQUIRED < 120000)) ||					\
				(defined (__WATCH_OS_VERSION_MIN_REQUIRED) &&					\
					(__WATCH_OS_VERSION_MIN_REQUIRED < 50000))))
			#define LUL_LIBCPP17_ANY											0L
			#define LUL_LIBCPP17_OPTIONAL										0L
			#define LUL_LIBCPP17_VARIANT										0L
		#endif

		//	Set up identifiers
		#if defined (__MAC_OS_X_VERSION_MIN_REQUIRED)
			#define LUL_TARGET_OS_IDENTIFIER u8"Min macOS version " LUL_Stringify_ (__MAC_OS_X_VERSION_MIN_REQUIRED)
		#elif defined (__IPHONE_OS_VERSION_MIN_REQUIRED)
			#define LUL_TARGET_OS_IDENTIFIER u8"Min iOS version " LUL_Stringify_ (__IPHONE_OS_VERSION_MIN_REQUIRED)
		#elif defined (__TV_OS_VERSION_MIN_REQUIRED)
			#define LUL_TARGET_OS_IDENTIFIER u8"Min tvOS version " LUL_Stringify_ (__TV_OS_VERSION_MIN_REQUIRED)
		#elif defined (__WATCH_OS_VERSION_MIN_REQUIRED)
			#define LUL_TARGET_OS_IDENTIFIER u8"Min watchOS version " LUL_Stringify_ (__WATCH_OS_VERSION_MIN_REQUIRED)
		#else
			#error "Unknown Apple platform"
		#endif
	#else
		#if LUL_TARGET_OS_OSX
			#define LUL_TARGET_OS_IDENTIFIER u8"macOS"
		#elif LUL_TARGET_OS_IOS
			#define LUL_TARGET_OS_IDENTIFIER u8"iOS"
		#elif LUL_TARGET_OS_IOS_SIM
			#define LUL_TARGET_OS_IDENTIFIER u8"iOS Simulator"
		#else
			#error "Unknown Apple platform"
		#endif
	#endif	//	Availability.h
#elif LUL_TARGET_OS_X11
	#ifndef LUL_TARGET_API_X11
		#define LUL_TARGET_API_X11					1
	#endif

	#if defined (__linux__)
		#define LUL_TARGET_OS_IDENTIFIER u8"Linux"
	#elif defined (__FreeBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"FreeBSD version " LUL_Stringify_ (__FreeBSD__)
	#elif defined (__NetBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"NetBSD version " LUL_Stringify_ (__NetBSD__)
	#elif defined (__OpenBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"OpenBSD version " LUL_Stringify_ (__OpenBSD__)
	#elif defined (__DragonFly__)
		#define LUL_TARGET_OS_IDENTIFIER u8"DragonFly version " LUL_Stringify_ (__DragonFly__)
	#else
		#error "Unknown LUL_TARGET_OS_X11 variant; update details/lulPlatformSetup.hpp"
	#endif
#elif LUL_TARGET_OS_WINAPI
	//	We make no assumptions about the addressing model based on processor,
	//	instead relying on intrinsics.
	#if defined (_WIN64)
		#define LUL_TARGET_RT_64_BIT				1

		#ifndef LUL_TARGET_API_WIN64
			#define LUL_TARGET_API_WIN64			1
		#endif

		#define LUL_TYPE_DOUBLE_GT_FLOAT			1
		#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		1
	#elif defined (_WIN32)
		#define LUL_TARGET_RT_32_BIT				1

		#ifndef LUL_TARGET_API_WIN32
			#define LUL_TARGET_API_WIN32			1
		#endif

		//	unverified
		#define LUL_TYPE_DOUBLE_GT_FLOAT			1
		#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		1
	#else
		#error "Unsupported addressing model"
	#endif


	#define LUL_FEATURE_UTF16_FILE_SYSTEM			1


	//	WinAPI configuration
	#define WIN32_LEAN_AND_MEAN
		//	FIXME Maybe not super-smart to set this up globally

	#define NOMINMAX


	#define LUL_TARGET_OS_IDENTIFIER u8"Windows"
#else
	#error "Unknown platform; update details/lulPlatformSetup.hpp"
#endif


/*------------------------------------------------------------------------------
	This -must- be defined by this point.
*/

#if !defined (LUL_TARGET_OS_IDENTIFIER)
	#error "LUL_TARGET_OS_IDENTIFIER must be defined"
#endif	//	LUL_TARGET_OS_IDENTIFIER


/*------------------------------------------------------------------------------
	Set up the rest of the OS conditionals
*/

#ifndef LUL_TARGET_OS_IOS
	#define LUL_TARGET_OS_IOS					0
#endif

#ifndef LUL_TARGET_OS_IOS_SIM
	#define LUL_TARGET_OS_IOS_SIM				0
#endif

#ifndef LUL_TARGET_OS_X11
	#define LUL_TARGET_OS_X11					0
#endif

#ifndef LUL_TARGET_OS_OSX
	#define LUL_TARGET_OS_OSX					0
#endif

#ifndef LUL_TARGET_OS_WIN
	#define LUL_TARGET_OS_WIN					0
#endif


/*------------------------------------------------------------------------------
	Set up TARGET_API values if they weren’t previously defined.
*/

#ifndef LUL_TARGET_API_COCOA
	#define LUL_TARGET_API_COCOA				0
#endif

#ifndef LUL_TARGET_API_COCOA_TOUCH
	#define LUL_TARGET_API_COCOA_TOUCH			0
#endif

#ifndef LUL_TARGET_API_WIN32
	#define LUL_TARGET_API_WIN32				0
#endif

#ifndef LUL_TARGET_API_WIN64
	#define LUL_TARGET_API_WIN64				0
#endif

#ifndef LUL_TARGET_API_X11
	#define LUL_TARGET_API_X11					0
#endif
