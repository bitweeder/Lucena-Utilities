/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Version.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <version> or <experimental/version>.
	This placeholder will select the correct header based on availability. The
	real <version> will define SD-6 macros of the form __cpp_lib_xxx; we don’t
	do that here. Instead, we include <version>, if it’s available, and then
	define our own macros that extend the SD-6 offering. See the notes under
	LUL_CPPxx in lulFeatureSetup.hpp for more information regarding macros,
	their names, and their values. Note that if __cpp_lib_xxx - or
	__cpp_lib_experimental_xxx - is set to 0, even if the corresponding header,
	if any, is found, we will ignore it and set the corresponding LUL_CPPxx -
	or LUL_LIBCPPxx_xxx_EXP - macro to 0; if the SD-6 macro is undefined, this
	caveat does not apply.

	Most LUL_LIBCPPxx_xxx macros have a corresponding LUL_LIBCPPxx_xxx_EXP
	macro; these latter macros are only set to non-zero values if an
	experimental header is being used. Note that LUL_LIBCPPxx_xxx and
	LUL_LIBCPPxx_xxx_EXP are mutually exclusive: if a Standard Library
	implementation ships with both regular and experimental versions of a
	given header, the experimental version will be ignored; if both
	LUL_LIBCPPxx_xxx and LUL_LIBCPPxx_xxx_EXP are defined to non-zero values,
	and the Standard header exists, LUL_LIBCPPxx_xxx_EXP will be set to 0 and
	a warning will be generated. If LUL_LIBCPPxx_xxx has been set to 0 and
	LUL_LIBCPPxx_xxx_EXP left undefined, LUL_LIBCPPxx_xxx_EXP will also be set
	to 0, even if the experimental feature in question is available; this is
	done to prevent surprises in the event that a feature was explicitly
	disabled, and then later an experimental version of the same feature is
	made available. The reverse, case, though, is not true; if left undefined,
	LUL_LIBCPPxx_xxx will always be assigned a value based on actual feature
	detection. Finally, note that if either macro is predefined to 0, its value
	will not be changed, regardless of circumstances.

	Note that this header should _always_ be used instead of - or in addition
	to - including <version> directly, even once the time comes that <version>
	is ubiquitous. The reason is that ultimately, <version> can only document
	intent, but it may fail to reflect reality. For example, there is no SD-6
	macro for “our internal policy precludes us from making changes to our C++
	stdlib runtime ABI between major OS releases, but our engineers forgot to
	remove the affected headers from our development tools releases for several
	years.”

	APIME Some of these depend on lulFeatureSetup.hpp being included, as they
	may need to be overridden based on an otherwise undetectable defect, e.g.,
	Apple’s failure to include required object code for <any> in most versions
	of its platforms’ runtime libraries.

	FIXME Add __cplusplus guards to header inclusion, as simple availability
	may be an insufficient test if an older std variant is being used.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>


//	First, set up any SD-6 macros, if available.
//	SEEME The use of <version> to contain SD-6 macros has not really been
//	formalized, yet, only its use as a replacement for <ciso646> abuse, as well
//	as a way to version the Standard Library implementation version. We’re
//	working under the asumption that this usage will be de facto, if not
//	formally approved.
#if !defined (LUL_LIBCPP2A_VERSION)
	#if __has_include (<version>)
		//	Note that we (probably) need to include <version> before
		//	__cpp_lib_version will have a value - if it is to have a value
		//	assigned at all. This is the very circular dependency <version> is
		//	intended to address, but first we need to prime it...
		#include <version>

		#if __cpp_lib_version || !defined (__cpp_lib_version)
			#if __cpp_lib_version
				#define LUL_LIBCPP2A_VERSION						__cpp_lib_version
			#else
				//	SEEME Value not yet assigned.
				#define LUL_LIBCPP2A_VERSION						1L
			#endif

			#ifndef LUL_CONFIG_std_version_supported
				#define LUL_CONFIG_std_version_supported			1
			#endif
		#else
			#define LUL_LIBCPP2A_VERSION							0L
		#endif
	#else
		#define LUL_LIBCPP2A_VERSION								0L
	#endif
#elif LUL_LIBCPP2A_VERSION
	#if !__has_include (<version>)
		#undef LUL_LIBCPP2A_VERSION
		#define LUL_LIBCPP2A_VERSION								0L
		#warning "<version> not found"
	#endif
#endif	//	LUL_LIBCPP2A_VERSION

#if !LUL_LIBCPP2A_VERSION
	//	In C++, this is a do-nothing header we include just for the side
	//	effects: the Standard Library implementation will be configured, and
	//	many assorted compiler- and library-dependent feature detection macros
	//	will be defined.
	#include <ciso646>
#endif

#ifndef LUL_CONFIG_std_version_supported
	#define LUL_CONFIG_std_version_supported						0
#endif	//	LUL_CONFIG_std_version_supported


//	A long-standing bug on older Apple platforms requires that we rely on
//	overrides to tell us whether this is really available, there. Note that
//	we neither track nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_ANY)
	#if __has_include (<any>) \
			&& (__cpp_lib_any || !defined (__cpp_lib_any))
		#if __cpp_lib_any
			#define LUL_LIBCPP17_ANY								__cpp_lib_any
		#else
			#define LUL_LIBCPP17_ANY								201606L
		#endif
	#else
		#define LUL_LIBCPP17_ANY									0L
	#endif
#elif LUL_LIBCPP17_ANY
	#if !__has_include (<any>)
		#undef LUL_LIBCPP17_ANY
		#define LUL_LIBCPP17_ANY									0L
		#warning "<any> not found"
	#endif
#endif	//	LUL_LIBCPP17_ANY


//	This functionality was originally going to be stuffed into <utility>, but
//	ended up getting its own header. Happily, this doesn’t end up impeding our
//	ability to detect the feature.
#if !defined (LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS)
	#if __has_include (<charconv>) \
			&& (__cpp_lib_to_chars || !defined (__cpp_lib_to_chars))
		#if __cpp_lib_to_chars
			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS		__cpp_lib_to_chars
		#else
			#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS		201611L
		#endif
	#else
		#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS			0L
	#endif
#elif LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS
	#if !__has_include (<charconv>)
		#undef LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS
		#define LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS			0L
		#warning "<charconv> not found"
	#endif
#endif	//	LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS


//	Use LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS to detect full
//	compliance with the feature; this only detects whether a key header is
//	implemented. Note that we neither track nor use experimental versions of
//	this.
#if !defined (LUL_LIBCPP17_EXECUTION)
	#if __has_include (<execution>) \
			&& (__cpp_lib_execution || !defined (__cpp_lib_execution))
		#if __cpp_lib_execution
			#define LUL_LIBCPP17_EXECUTION							__cpp_lib_execution
		#else
			#define LUL_LIBCPP17_EXECUTION							201603L
		#endif
	#else
		#define LUL_LIBCPP17_EXECUTION								0L
	#endif
#elif LUL_LIBCPP17_EXECUTION
	#if !__has_include (<execution>)
		#undef LUL_LIBCPP17_EXECUTION
		#define LUL_LIBCPP17_EXECUTION								0L
		#warning "<execution> not found"
	#endif
#endif	//	LUL_LIBCPP17_EXECUTION


//	Only missing on Apple platforms; given the significant number of missed
//	opportunities to address this, it may never arrive.
#if !defined (LUL_LIBCPP17_FILESYSTEM)
	#if __has_include (<filesystem>) && !LUL_LIBCPP17_FILESYSTEM_EXP \
			&& (__cpp_lib_filesystem || !defined (__cpp_lib_filesystem))
		#if __cpp_lib_filesystem
			#define LUL_LIBCPP17_FILESYSTEM							__cpp_lib_filesystem
		#else
			#define LUL_LIBCPP17_FILESYSTEM							201703L
		#endif

		#ifndef LUL_LIBCPP17_FILESYSTEM_EXP
			#define LUL_LIBCPP17_FILESYSTEM_EXP						0L
		#endif
	#else
		#define LUL_LIBCPP17_FILESYSTEM								0L
	#endif
#elif LUL_LIBCPP17_FILESYSTEM
	#if !__has_include (<filesystem>)
		#undef LUL_LIBCPP17_FILESYSTEM
		#define LUL_LIBCPP17_FILESYSTEM								0L
		#warning "<filesystem> not found"
	#endif
#else
	#if !defined (LUL_LIBCPP17_FILESYSTEM_EXP)
		#define LUL_LIBCPP17_FILESYSTEM_EXP							0L
	#endif
#endif	//	LUL_LIBCPP17_FILESYSTEM

#if !defined (LUL_LIBCPP17_FILESYSTEM_EXP)
	#if __has_include (<experimental/filesystem>) \
			&& (__cpp_lib_experimental_filesystem || !defined (__cpp_lib_experimental_filesystem))
		#if __cpp_lib_experimental_filesystem
			#define LUL_LIBCPP17_FILESYSTEM_EXP						__cpp_lib_experimental_filesystem
		#else
			#define LUL_LIBCPP17_FILESYSTEM_EXP						201406L
		#endif
	#else
		#define LUL_LIBCPP17_FILESYSTEM_EXP							0L
	#endif
#elif LUL_LIBCPP17_FILESYSTEM_EXP
	#if LUL_LIBCPP17_FILESYSTEM
		#undef LUL_LIBCPP17_FILESYSTEM_EXP
		#define LUL_LIBCPP17_FILESYSTEM_EXP							0L
		#warning "LUL_LIBCPP17_FILESYSTEM && LUL_LIBCPP17_FILESYSTEM_EXP"
	#elif !__has_include (<experimental/filesystem>)
		#undef LUL_LIBCPP17_FILESYSTEM_EXP
		#define LUL_LIBCPP17_FILESYSTEM_EXP							0L
		#warning "<experimental/filesystem> not found"
	#endif
#endif	//	LUL_LIBCPP17_FILESYSTEM_EXP


//	This functionality lives in <new>, so we have to rely on either the SD-6
//	macro or an override to detect it; if no explicit override is set and
//	the SD-6 macro is unavailable, we default to 0. Note that we neither track
//	nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE)
	#if __cpp_lib_thread_hardware_interference_size
		#define LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE				__cpp_lib_thread_hardware_interference_size
	#else
		#define LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE				0L
	#endif
#endif	//	LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE


//	This functionality lives in <new>, so we have to rely on either the SD-6
//	macro or an override to detect it; if no explicit override is set and
//	the SD-6 macro is unavailable, we default to 0. Note that we neither track
//	nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_LAUNDER)
	#if __cpp_lib_launder
		#define LUL_LIBCPP17_LAUNDER								__cpp_lib_launder
	#else
		#define LUL_LIBCPP17_LAUNDER								0L
	#endif
#endif	//	LUL_LIBCPP17_LAUNDER


//	A long-standing bug on older Apple platforms requires that we rely on
//	overrides to tell us whether this is really available, there. Note that
//	we neither track nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_OPTIONAL)
	#if __has_include (<optional>) \
			&& (__cpp_lib_optional || !defined (__cpp_lib_optional))
		#if __cpp_lib_optional
			#define LUL_LIBCPP17_OPTIONAL							__cpp_lib_optional
		#else
			#define LUL_LIBCPP17_OPTIONAL							201606L
		#endif
	#else
		#define LUL_LIBCPP17_OPTIONAL								0L
	#endif
#elif LUL_LIBCPP17_OPTIONAL
	#if !__has_include (<optional>)
		#undef LUL_LIBCPP17_OPTIONAL
		#define LUL_LIBCPP17_OPTIONAL								0L
		#warning "<optional> not found"
	#endif
#endif	//	LUL_LIBCPP17_OPTIONAL


//	Use LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS to detect full
//	compliance with the feature; this only detects whether the requisite
//	algorithms have been implemented. Note that as this functionality lives in
//	multiple pre-existing std headers, so we have to rely on either the SD-6
//	macro or an override to detect it; without these, we default to 0. Note
//	that we neither track nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_PARALLEL_ALGORITHM)
	#if __cpp_lib_parallel_algorithm
		#define LUL_LIBCPP17_PARALLEL_ALGORITHM						__cpp_lib_parallel_algorithm
	#else
		#define LUL_LIBCPP17_PARALLEL_ALGORITHM						0L
	#endif
#endif	//	LUL_LIBCPP17_PARALLEL_ALGORITHM


//	This functionality lives in multiple pre-existing std headers, so we have
//	to rely on either the SD-6 macro or an override to detect it; without
//	these, we default to 0. Note that we neither track nor use experimental
//	versions of this.
#if !defined (LUL_LIBCPP17_SPLICING_MAPS_AND_SETS)
	#if __cpp_lib_node_extract
		#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS					__cpp_lib_node_extract
	#else
		#define LUL_LIBCPP17_SPLICING_MAPS_AND_SETS					0L
	#endif
#endif	//	LUL_LIBCPP17_SPLICING_MAPS_AND_SETS


//	This is an aggregate that tracks two different SD-6 macros for the purpose
//	of establishing Standard compliance. There is no single corresponding SD-6
//	macro. Note that we neither track nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS)
	#if LUL_LIBCPP17_EXECUTION && LUL_LIBCPP17_PARALLEL_ALGORITHM
		#define LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS		1L
	#else
		#define LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS		0L
	#endif
#endif	//	LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS


//	A long-standing bug on older Apple platforms requires that we rely on
//	overrides to tell us whether this is really available, there. Note that
//	we neither track nor use experimental versions of this.
#if !defined (LUL_LIBCPP17_VARIANT)
	#if __has_include (<variant>) \
			&& (__cpp_lib_variant || !defined (__cpp_lib_variant))
		#if __cpp_lib_variant
			#define LUL_LIBCPP17_VARIANT							__cpp_lib_variant
		#else
			#define LUL_LIBCPP17_VARIANT							201606L
		#endif
	#else
		#define LUL_LIBCPP17_VARIANT								0L
	#endif
#elif LUL_LIBCPP17_VARIANT
	#if !__has_include (<variant>)
		#undef LUL_LIBCPP17_VARIANT
		#define LUL_LIBCPP17_VARIANT								0L
		#warning "<variant> not found"
	#endif
#endif	//	LUL_LIBCPP17_VARIANT


//	This functionality lives in <bit>, but requires either the SD-6 macro or an
//	override to detect it; if no explicit override is set and the SD-6 macro is
//	unavailable, we default to 0.
#if !defined (LUL_LIBCPP2A_BIT_CAST)
	#if __has_include (<bit>) \
			&& (__cpp_lib_bit_cast || !defined (__cpp_lib_bit_cast))
		#if __cpp_lib_bit_cast
			#define LUL_LIBCPP2A_BIT_CAST							__cpp_lib_bit_cast
		#else
			#define LUL_LIBCPP2A_BIT_CAST							201806L
		#endif
	#else
		#define LUL_LIBCPP2A_BIT_CAST								0L
	#endif
#elif LUL_LIBCPP2A_BIT_CAST
	#if !__has_include (<bit>)
		#undef LUL_LIBCPP2A_BIT_CAST
		#define LUL_LIBCPP2A_BIT_CAST								0L
		#warning "<bit> not found"
	#endif
#endif	//	LUL_LIBCPP2A_BIT_CAST


//	This functionality lives in <chrono>, but it currently has no SD-6 macro
//	and requires an override to detect it; if no explicit override is set, we
//	default to 0.
#if !defined (LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE)
	#define LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE						0L
#endif	//	LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE


//	Note that this for the Concepts support library; the language feature is
//	handled separately.
#if !defined (LUL_LIBCPP2A_CONCEPT_LIBRARY)
	#if __has_include (<concepts>) && !LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP \
			&& (__cpp_lib_concepts || !defined (__cpp_lib_concepts))
		#if __cpp_lib_concepts
			#define LUL_LIBCPP2A_CONCEPT_LIBRARY					__cpp_lib_concepts
		#else
			#define LUL_LIBCPP2A_CONCEPT_LIBRARY					201806L
		#endif

		#ifndef LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
			#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP				0L
		#endif
	#else
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY						0L
	#endif
#elif LUL_LIBCPP2A_CONCEPT_LIBRARY
	#if !__has_include (<concepts>)
		#undef LUL_LIBCPP2A_CONCEPT_LIBRARY
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY						0L
		#warning "<concepts> not found"
	#endif
#else
	#if !defined (LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP)
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP					0L
	#endif
#endif	//	LUL_LIBCPP2A_CONCEPT_LIBRARY

#if !defined (LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP)
	#if __has_include (<experimental/concepts>) \
			&& (__cpp_lib_experimental_concepts || !defined (__cpp_lib_experimental_concepts))
		#if __cpp_lib_experimental_concepts
			#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP				__cpp_lib_experimental_concepts
		#else
			#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP				201507L
		#endif
	#else
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP					0L
	#endif
#elif LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
	#if LUL_LIBCPP2A_CONCEPT_LIBRARY
		#undef LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP					0L
		#warning "LUL_LIBCPP2A_CONCEPT_LIBRARY && LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP"
	#elif !__has_include (<experimental/concepts>)
		#undef LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
		#define LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP					0L
		#warning "<experimental/concepts> not found"
	#endif
#endif	//	LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP


//	This functionality lives in multiple pre-existing headers, and requires
//	either the SD-6 macro or an override to detect it; if no explicit override
//	is set and the SD-6 macro is unavailable, we default to 0.
#if !defined (LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY)
	#if __cpp_lib_constexpr_algorithms
		#define LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY	__cpp_lib_constexpr_algorithms
	#else
		#define LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY	0L
	#endif
#endif	//	LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY


//	SEEME This was originally a TS (with competing versions), and so has
//	experimental implementations we track.
//	SEEME Note that there is no separate SD-6 macro for the library header vs.
//	the language feature, so we just use language feature macro; this will
//	almost definitely change to __cpp_lib_coroutine in the future, but until it
//	does, we’re following the approved proposal.
#if !defined (LUL_LIBCPP2A_COROUTINES)
	#if __has_include (<coroutine>) && !LUL_LIBCPP2A_COROUTINES_EXP \
			&& (__cpp_coroutines || !defined (__cpp_coroutines))
		#if __cpp_coroutines
			#define LUL_LIBCPP2A_COROUTINES							__cpp_coroutines
		#else
			#define LUL_LIBCPP2A_COROUTINES							201803L
		#endif

		#ifndef LUL_LIBCPP2A_COROUTINES_EXP
			#define LUL_LIBCPP2A_COROUTINES_EXP						0L
		#endif
	#else
		#define LUL_LIBCPP2A_COROUTINES								0L
	#endif
#elif LUL_LIBCPP2A_COROUTINES
	#if !__has_include (<coroutine>)
		#undef LUL_LIBCPP2A_COROUTINES
		#define LUL_LIBCPP2A_COROUTINES								0L
		#warning "<coroutine> not found"
	#endif
#else
	#if !defined (LUL_LIBCPP2A_COROUTINES_EXP)
		#define LUL_LIBCPP2A_COROUTINES_EXP							0L
	#endif
#endif	//	LUL_LIBCPP2A_COROUTINES

#if !defined (LUL_LIBCPP2A_COROUTINES_EXP)
	#if __has_include (<experimental/coroutine>) \
			&& (__cpp_coroutines || !defined (__cpp_coroutines))
		#if __cpp_coroutines
			#define LUL_LIBCPP2A_COROUTINES_EXP						__cpp_coroutines
		#else
			#define LUL_LIBCPP2A_COROUTINES_EXP						201803L
		#endif
	#else
		#define LUL_LIBCPP2A_COROUTINES_EXP							0L
	#endif
#elif LUL_LIBCPP2A_COROUTINES_EXP
	#if LUL_LIBCPP2A_COROUTINES
		#undef LUL_LIBCPP2A_COROUTINES_EXP
		#define LUL_LIBCPP2A_COROUTINES_EXP							0L
		#warning "LUL_LIBCPP2A_COROUTINES && LUL_LIBCPP2A_COROUTINES_EXP"
	#elif !__has_include (<experimental/coroutine>)
		#undef LUL_LIBCPP2A_COROUTINES_EXP
		#define LUL_LIBCPP2A_COROUTINES_EXP							0L
		#warning "<experimental/coroutine> not found"
	#endif
#endif	//	LUL_LIBCPP2A_COROUTINES_EXP


//	This functionality lives in <memory>, but it currently has no SD-6 macro
//	and requires an override to detect it; if no explicit override is set, we
//	default to 0.
#if !defined (LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS)
	#define LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS	0L
#endif	//	LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS


//	This functionality lives in <atomic>, but it currently has no SD-6 macro
//	and requires an override to detect it; if no explicit override is set, we
//	default to 0.
#if !defined (LUL_LIBCPP2A_FLOATING_POINT_ATOMIC)
	#define LUL_LIBCPP2A_FLOATING_POINT_ATOMIC						0L
#endif	//	LUL_LIBCPP2A_FLOATING_POINT_ATOMIC


//	This functionality lives in multiple pre-existing headers, and requires
//	either the SD-6 macro or an override to detect it; if no explicit override
//	is set and the SD-6 macro is unavailable, we default to 0.
#if !defined (LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE)
	#if __cpp_lib_list_remove_return_type
		#define LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE				__cpp_lib_list_remove_return_type
	#else
		#define LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE				0L
	#endif
#endif	//	LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE


//	This functionality lives in <complex>, and requires either the SD-6 macro
//	or an override to detect it; if no explicit override is set and the SD-6
//	macro is unavailable, we default to 0.
#if !defined (LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX)
	#if __cpp_lib_constexpr_complex
		#define LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX				__cpp_lib_constexpr_complex
	#else
		#define LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX				0L
	#endif
#endif	//	LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX


//	There is no SD-6 macro, yet.
#if !defined (LUL_LIBCPP2A_SPAN)
	#if __has_include (<span>)
		//	SEEME Value not yet assigned.
		#define LUL_LIBCPP2A_SPAN									1L
	#else
		#define LUL_LIBCPP2A_SPAN									0L
	#endif
#elif LUL_LIBCPP2A_SPAN
	#if !__has_include (<span>)
		#undef LUL_LIBCPP2A_SPAN
		#define LUL_LIBCPP2A_SPAN									0L
		#warning "<span> not found"
	#endif
#endif	//	LUL_LIBCPP2A_SPAN


//	This functionality lives in <atomic>, and requires either the SD-6 macro
//	or an override to detect it; if no explicit override is set and the SD-6
//	macro is unavailable, we default to 0.
#if !defined (LUL_LIBCPP2A_STD_ATOMIC_REF)
	#if __cpp_lib_atomic_ref
		#define LUL_LIBCPP2A_STD_ATOMIC_REF							__cpp_lib_atomic_ref
	#else
		#define LUL_LIBCPP2A_STD_ATOMIC_REF							0L
	#endif
#endif	//	LUL_LIBCPP2A_STD_ATOMIC_REF


//	This functionality lives in <type_traits>, but it currently has no SD-6
//	macro and requires an override to detect it; if no explicit override is
//	set, we default to 0.
#if !defined (LUL_LIBCPP2A_STD_ENDIAN)
	#define LUL_LIBCPP2A_STD_ENDIAN									0L
#endif	//	LUL_LIBCPP2A_STD_ENDIAN


//	This functionality lives in <type_traits>, but it currently has no SD-6
//	macro and requires an override to detect it; if no explicit override is
//	set, we default to 0.
#if !defined (LUL_LIBCPP2A_STD_REMOVE_CVREF)
	#define LUL_LIBCPP2A_STD_REMOVE_CVREF							0L
#endif	//	LUL_LIBCPP2A_STD_REMOVE_CVREF


//	This functionality lives in <string> and <string_view>, but it currently
//	has no SD-6 macro and requires an override to detect it; if no explicit
//	override is set, we default to 0.
#if !defined (LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING)
	#define LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING			0L
#endif	//	LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING


#if !defined (LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM)
	#if __has_include (<syncstream>) \
			&& (__cpp_lib_syncstream || !defined (__cpp_lib_syncstream))
		#if __cpp_lib_syncstream
			#define LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM		__cpp_lib_syncstream
		#else
			//	SEEME Value not yet assigned.
			#define LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM		1L
		#endif
	#else
		#define LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM			0L
	#endif
#elif LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM
	#if !__has_include (<syncstream>)
		#undef LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM
		#define LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM			0L
		#warning "<syncstream> not found"
	#endif
#endif	//	LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM


//	There is no SD-6 macro, yet.
#if !defined (LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE)
	#if __has_include (<compare>)
		//	SEEME Value not yet assigned.
		#define LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE	1L
	#else
		#define LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE	0L
	#endif
#elif LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE
	#if !__has_include (<compare>)
		#undef LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE
		#define LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE	0L
		#warning "<compare> not found"
	#endif
#endif	//	LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE


//	This functionality lives in <type_traits>, but it currently has no SD-6
//	macro and requires an override to detect it; if no explicit override is
//	set, we default to 0.
#if !defined (LUL_LIBCPP2A_TYPE_IDENTITY)
	#define LUL_LIBCPP2A_TYPE_IDENTITY								0L
#endif	//	LUL_LIBCPP2A_TYPE_IDENTITY


//	This functionality lives in <memory>, but it currently has no SD-6 macro
//	and requires an override to detect it; if no explicit override is set, we
//	default to 0.
#if !defined (LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER)
	#define LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER	0L
#endif	//	LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER


//	Networking functionality spans an array of new headers; we only check for
//	the presence of the convenience header, <net>.
#if !defined (LUL_LIBCPPTS_NETWORKING)
	#if __has_include (<net>) && !LUL_LIBCPPTS_NETWORKING_EXP \
			&& (__cpp_lib_net || !defined (__cpp_lib_net))
		#if __cpp_lib_net
			#define LUL_LIBCPPTS_NETWORKING							__cpp_lib_net
		#else
			//	SEEME This value is a proxy; the Networking TS has not yet been
			//	approved.
			#define LUL_LIBCPPTS_NETWORKING							201803L
		#endif

		#ifndef LUL_LIBCPPTS_NETWORKING_EXP
			#define LUL_LIBCPPTS_NETWORKING_EXP						0L
		#endif
	#else
		#define LUL_LIBCPPTS_NETWORKING								0L
	#endif
#elif LUL_LIBCPPTS_NETWORKING
	#if !__has_include (<net>)
		#undef LUL_LIBCPPTS_NETWORKING
		#define LUL_LIBCPPTS_NETWORKING								0L
		#warning "<net> not found"
	#endif
#else
	#if !defined (LUL_LIBCPPTS_NETWORKING_EXP)
		#define LUL_LIBCPPTS_NETWORKING_EXP							0L
	#endif
#endif	//	LUL_LIBCPPTS_NETWORKING

#if !defined (LUL_LIBCPPTS_NETWORKING_EXP)
	#if __has_include (<experimental/net>) \
			&& (__cpp_lib_experimental_net || !defined (__cpp_lib_experimental_net))
		#if __cpp_lib_experimental_net
			#define LUL_LIBCPPTS_NETWORKING_EXP						__cpp_lib_experimental_net
		#else
			#define LUL_LIBCPPTS_NETWORKING_EXP						201803L
		#endif
	#else
		#define LUL_LIBCPPTS_NETWORKING_EXP							0L
	#endif
#elif LUL_LIBCPPTS_NETWORKING_EXP
	#if LUL_LIBCPPTS_NETWORKING
		#undef LUL_LIBCPPTS_NETWORKING_EXP
		#define LUL_LIBCPPTS_NETWORKING_EXP							0L
		#warning "LUL_LIBCPPTS_NETWORKING && LUL_LIBCPPTS_NETWORKING_EXP"
	#elif !__has_include (<experimental/net>)
		#undef LUL_LIBCPPTS_NETWORKING_EXP
		#define LUL_LIBCPPTS_NETWORKING_EXP							0L
		#warning "<experimental/net> not found"
	#endif
#endif	//	LUL_LIBCPPTS_NETWORKING_EXP


//	Networking functionality spans an array of new headers; we only check for
//	the presence of the convenience header, <net>.
//	FIXME this may be incorrect for the networking extensions; check the TS.
#if !defined (LUL_LIBCPPTS_NETWORKING_EXTENSIBLE)
	#if __has_include (<net>) && !LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP \
			&& (__cpp_lib_net_extensible || !defined (__cpp_lib_net_extensible))
		#if __cpp_lib_net_extensible
			#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE				__cpp_lib_net_extensible
		#else
			//	SEEME This value is a proxy; the Networking TS has not yet been
			//	approved.
			#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE				201803L
		#endif

		#ifndef LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP
			#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP			0L
		#endif
	#else
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE					0L
	#endif
#elif LUL_LIBCPPTS_NETWORKING_EXTENSIBLE
	#if !__has_include (<net>)
		#undef LUL_LIBCPPTS_NETWORKING_EXTENSIBLE
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE					0L
		#warning "<net> not found"
	#endif
#else
	#if !defined (LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP)
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP				0L
	#endif
#endif	//	LUL_LIBCPPTS_NETWORKING_EXTENSIBLE

#if !defined (LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP)
	#if __has_include (<experimental/net>) \
			&& (__cpp_lib_experimental_net_extensible || !defined (__cpp_lib_experimental_net_extensible))
		#if __cpp_lib_experimental_net_extensible
			#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP			__cpp_lib_experimental_net_extensible
		#else
			#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP			201803L
		#endif
	#else
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP				0L
	#endif
#elif LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP
	#if LUL_LIBCPPTS_NETWORKING_EXTENSIBLE
		#undef LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP				0L
		#warning "LUL_LIBCPPTS_NETWORKING_EXTENSIBLE && LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP"
	#elif !__has_include (<experimental/net>)
		#undef LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP
		#define LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP				0L
		#warning "<experimental/net> not found"
	#endif
#endif	//	LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP


//	This functionality lives in <memory>, and requires either the SD-6 macro
//	or an override to detect it; if no explicit override is set and the SD-6
//	macro is unavailable, we default to 0.
//	SEEME It might make sense to check for the existence of an experimental
//	version of <memory> when using __cpp_lib_experimental_observer_ptr. For
//	now, we don’t over-think it since there are no examples in the wild to
//	consider.
#if !defined (LUL_LIBCPPTS_OBSERVER_PTR)
	#if !LUL_LIBCPPTS_OBSERVER_PTR_EXP && __cpp_lib_observer_ptr
		#define LUL_LIBCPPTS_OBSERVER_PTR							__cpp_lib_observer_ptr

		#ifndef LUL_LIBCPPTS_OBSERVER_PTR_EXP
			#define LUL_LIBCPPTS_OBSERVER_PTR_EXP					0L
		#endif
	#else
		#define LUL_LIBCPPTS_OBSERVER_PTR							0L
	#endif
#elif !LUL_LIBCPPTS_OBSERVER_PTR
	#if !defined (LUL_LIBCPPTS_OBSERVER_PTR_EXP)
		#define LUL_LIBCPPTS_OBSERVER_PTR_EXP						0L
	#endif
#endif	//	LUL_LIBCPPTS_OBSERVER_PTR

#if !defined (LUL_LIBCPPTS_OBSERVER_PTR_EXP)
	#if __cpp_lib_experimental_observer_ptr
		#define LUL_LIBCPPTS_OBSERVER_PTR_EXP						__cpp_lib_experimental_observer_ptr
	#else
		#define LUL_LIBCPPTS_OBSERVER_PTR_EXP						0L
	#endif
#elif LUL_LIBCPPTS_OBSERVER_PTR_EXP
	#if LUL_LIBCPPTS_OBSERVER_PTR
		#undef LUL_LIBCPPTS_OBSERVER_PTR_EXP
		#define LUL_LIBCPPTS_OBSERVER_PTR_EXP						0L
		#warning "LUL_LIBCPPTS_OBSERVER_PTR && LUL_LIBCPPTS_OBSERVER_PTR_EXP"
	#endif
#endif	//	LUL_LIBCPPTS_OBSERVER_PTR_EXP
