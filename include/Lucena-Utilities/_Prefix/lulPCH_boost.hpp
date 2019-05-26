/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“PCH_boost.hpp”
	Copyright © 2006-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This header is intended for use as a precompiled header, or at least a
	component of one. It is also suitable for use as a prefix header. We make
	no assumptions about previously included headers here; in particular, there
	is no reliance on precompiled headers or other implicitly included headers.

------------------------------------------------------------------------------*/


#pragma once

//	Boost headers are chock-full of “won’t fix” warnings that muddle output,
//	so we hide them all by default. The downside is that this may mask a
//	legitimate problem, but that’s just another good reason to minimize boost
//	dependencies.
#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


//#include <boost/config.hpp>


//	SEEME Certain boost headers may be included by wrapper headers to provide
//	functionality currently missing from the local Standard Library. Note that
//	this means these headers may be included in the STL PCH header, which is
//	actually a bit awkward. At least it’s temporary...


//	The boost.asio headers are included by lulNetworkingWrapper.hpp if
//	applicable, so do not need to be included here.
//#include <boost/asio/buffer.hpp>
//#include <boost/asio/io_service.hpp>
//#include <boost/asio/write.hpp>
//
//#include <boost/asio/ip/tcp.hpp>
//#include <boost/asio/ip/udp.hpp>


//	The boost.filesystem headers are included by lulFilesystemWrapper.hpp if
//	applicable, so do not need to be included here.
//#include <boost/filesystem.hpp>
//#include <boost/filesystem/fstream.hpp>
//#include <boost/filesystem/path.hpp>


//	The boost.iostreams library is not currently being used by
//	Lucena Utilities.
//#include <boost/iostreams/filter/bzip2.hpp>
//#include <boost/iostreams/filter/gzip.hpp>
//#include <boost/iostreams/filter/zlib.hpp>
//#include <boost/iostreams/filtering_streambuf.hpp>
//#include <boost/iostreams/tee.hpp>


//#include <boost/system/error_code.hpp>


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
