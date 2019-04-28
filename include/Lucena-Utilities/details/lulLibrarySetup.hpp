/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“LibrarySetup.hpp”
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

	Macros that we set are our responsiblity; they should not be set outside of
	this header. Since none of the LUL_XXX macros we’re responsible for are set
	until after the library tests, it’s sufficient to simply test them for
	existence to see if they’ve been improperly defined, with the following
	exceptions:

		- LUL_LIBCPPxx_xxx macros are only set here if it would not be possible
			otherwise to set them properly in lulVersion.hpp (a small subset
			are potentially defined in lulPlatformSetup.hpp, but that’s very
			uncommon). See the notes in the documentation for the affected
			macros and in lulVersion.hpp.
*/

#if defined (LUL_NAME_STANDARD_LIBRARY)
	#error "Don’t define LUL_NAME_STANDARD_LIBRARY outside of this file."
#endif	//	LUL_NAME_STANDARD_LIBRARY


/*------------------------------------------------------------------------------
	Library Macros

	SEEME Arguably, it would be more desirable to give each Standard Library
	implementation its own #include file and include them here conditionally
	rather than go the monolithic route. We trade off potential unnecessary
	recompilation of unaffected projects for simplified maintenance at this
	time.
*/

#if defined (_LIBCPP_VERSION)
	#if defined (__apple_build_version__)
		//	SEEME We use this as a proxy for detecting Apple’s hacked-up
		//	version of libc++; there doesn’t appear to be a reliable way to
		//	actually determine this.

		//	SEEME Speculating about future availability in Apple’s libc++ is
		//	fraught, as features fail to be adopted from vanilla clang for any
		//	number of reasons (e.g., std::any, std::filesystem), and oddities
		//	persist for years (e.g., std::is_callable vs std::is_invocable). We
		//	assume nothing, and wait.
		#if (_LIBCPP_VERSION >= 6000)
//			#if !__cpp_lib_launder
//				#define LUL_LIBCPP17_LAUNDER									201606L
//			#endif
//
//			#define LUL_LIBCPP2A_STD_REMOVE_CVREF								1L
//			#define LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING				1L
//			#define LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER	1L
		#endif

		#if (_LIBCPP_VERSION >= 7000)
//			#define LUL_LIBCPP2A_STD_ENDIAN										1L
		#endif
	#else
		#if (_LIBCPP_VERSION >= 6000)
			#if !__cpp_lib_launder
				#define LUL_LIBCPP17_LAUNDER									201606L
			#endif

			#define LUL_LIBCPP2A_STD_REMOVE_CVREF								1L
			#define LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING				1L
			#define LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER	1L
		#endif

		#if (_LIBCPP_VERSION >= 7000)
			#define LUL_LIBCPP2A_STD_ENDIAN										1L

			//	SEEME In-progress
//			#define LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY			1L
//			#define LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX						1L

			//	SEEME This appears to be in, though we await confirmation.
			#define LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE							1L
		#endif

		#if (_LIBCPP_VERSION >= 8000)
			#define LUL_LIBCPP2A_TYPE_IDENTITY									1L
		#endif
	#endif


	//	Set up identifiers
	#define LUL_NAME_STANDARD_LIBRARY u8"libc++ version " LUL_Stringify_ (_LIBCPP_VERSION)
#elif defined (__GLIBCXX__)
	//	SEEME Note that older iterations of libstdc++ used __GLIBCPP__

	#if (__GNUC__ >= 7)
		#if __cpp_lib_launder
			#define LUL_LIBCPP17_LAUNDER						__cpp_lib_launder
		#else
			#define LUL_LIBCPP17_LAUNDER						201606L
		#endif

		#if __cpp_lib_node_extract
			#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS			__cpp_lib_node_extract
		#else
			#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS			201606L
		#endif
	#endif

	#if (__GNUC__ >= 8)
		//	SEEME Only ints are supported; floats are forthcoming.
//		#if __cpp_lib_to_chars
//			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS	__cpp_lib_to_chars
//		#else
//			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS	201611L
//		#endif
		#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS		0L

		#define LUL_LIBCPP2A_STD_ENDIAN							1L
		#define LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER	1L
	#endif

	#if (__GNUC__ >= 9)
		#define LUL_LIBCPP2A_STD_REMOVE_CVREF					1L
	#endif


	//	Set up identifiers
	#define LUL_NAME_STANDARD_LIBRARY u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCXX__)
#elif defined (_MSC_VER)
	//	SEEME Not exactly an equivalent test, but I don’t know of a documented
	//	way to reliably identify the MSVC Standard Library.

	#if (_MSC_VER >= 1911)
		#if __cpp_lib_thread_hardware_interference_size
			#define LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE		__cpp_lib_thread_hardware_interference_size
		#else
			#define LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE		201703L
		#endif
	#endif

	#if (_MSC_VER >= 1912)
		#if __cpp_lib_node_extract
			#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS			__cpp_lib_node_extract
		#else
			#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS			201606L
		#endif
	#endif

	#if (_MSC_VER >= 1914)
		#if __cpp_lib_launder
			#define LUL_LIBCPP17_LAUNDER						__cpp_lib_launder
		#else
			#define LUL_LIBCPP17_LAUNDER						201606L
		#endif

		#if __cpp_lib_parallel_algorithm
			#define LUL_LIBCPP17_PARALLEL_ALGORITHM				__cpp_lib_parallel_algorithm
		#else
			#define LUL_LIBCPP17_PARALLEL_ALGORITHM				201603L
		#endif

		//	SEEME There is partial support for this, but only for ints; floats
		//	are being actively worked on.
//		#if __cpp_lib_to_chars
//			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS	__cpp_lib_to_chars
//		#else
//			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS	201611L
//		#endif
		#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS		0L
	#endif

	//	Set up identifiers
	#define LUL_NAME_STANDARD_LIBRARY u8"MSVC Standard Library version" LUL_Stringify_ (_MSC_VER)
#else
	#define LUL_NAME_STANDARD_LIBRARY u8"Unknown Standard Library implementation"
#endif


/*------------------------------------------------------------------------------
	This -must- be defined by this point.
*/

#if !defined (LUL_NAME_STANDARD_LIBRARY)
	#error "LUL_NAME_STANDARD_LIBRARY must be defined"
#endif	//	LUL_NAME_STANDARD_LIBRARY

