/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“NetworkingWrapper.hpp”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <networking>, <experimental/networking>,
	or the boost ASIO networking-related headers directly. This placeholder 
	will select the correct headers based on availability.

	FIXME All the type aliases and namespace imports are unimplemented, so
	this header is really just a placeholder. Additional complications are due
	to language additions necessary to fully support the proposal and the
	dependency on coroutines, which finally made their way into the Standard,
	but also require language additions to function correctly.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <Lucena-Utilities/lulVersionWrapper.hpp>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>


//	SEEME This is a little hinky since the interfaces between the std and boost
//	implementations differ in a number of places. For this reason, we have to
//	be explicit about which one we’re supporting.
//	APIME The std versions only guarantee support for UDP and TCP, so we
//	restrict ourselves to that.
#ifdef LUL_TEMP_NETWORKING_NAMESPACE
	#error "LUL_TEMP_NETWORKING_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_boost_networking
	#if LUL_LIBCPPTS_NETWORKING
		#include <networking>

		#define LUL_CONFIG_std_neworking_supported 1

		#define LUL_TEMP_NETWORKING_NAMESPACE std
	#elif LUL_LIBCPPTS_NETWORKING_EXP
		#include <experimental/networking>

		#define LUL_CONFIG_std_neworking_supported 1

		#define LUL_TEMP_NETWORKING_NAMESPACE std::experimental
	#endif
#endif

#if !defined (LUL_TEMP_NETWORKING_NAMESPACE) && LUL_CONFIG_use_boost_networking
	//	Boost headers are chock-full of “won’t fix” warnings that muddle
	//	output, so we hide them all by default. The downside is that this may
	//	mask a legitimate problem, but that’s just another good reason to
	//	minimize boost dependencies.
	#if __GNUC__ >= 4
		#pragma GCC system_header
	#elif defined (_MSC_VER) && defined (_WIN32)
		#pragma warning (push, 0)
	#endif

	//	boost doesn’t offer a networking-specific convenience header, so we
	//	have to explicitly include all relevant headers. Note that we could
	//	just include <boost/asio.hpp> here, but that seems seriously excessive.
//	#include <boost/asio/buffer.hpp>
//	#include <boost/asio/io_service.hpp>
//	#include <boost/asio/write.hpp>
//
//	#include <boost/asio/ip/tcp.hpp>
//	#include <boost/asio/ip/udp.hpp>

	#define LUL_CONFIG_boost_neworking_supported 1

	#define LUL_TEMP_NETWORKING_NAMESPACE boost

	#if defined (_MSC_VER) && defined (_WIN32)
		#pragma warning (pop)
	#endif
#endif

#ifdef LUL_TEMP_NETWORKING_NAMESPACE
	LUL_begin_v_namespace

	//	FIXME Aliases go here

	LUL_end_v_namespace

	#undef LUL_TEMP_NETWORKING_NAMESPACE
#else
	#error "networking support is unavailable"
#endif

#ifndef LUL_CONFIG_std_neworking_supported
	#define LUL_CONFIG_std_neworking_supported 0
#endif	//	LUL_CONFIG_std_neworking_supported

#ifndef LUL_CONFIG_boost_neworking_supported
	#define LUL_CONFIG_boost_neworking_supported 0
#endif	//	LUL_CONFIG_boost_neworking_supported
