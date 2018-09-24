/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Lucena-Utilities.cpp”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	SEEME This file is gratuitous right now. Lucena Utilities is currently a
	header-only library.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only

#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulVersion.hpp>

#include "lulConfig_priv.hpp"
	//	SEEME Note that this must be included before Lucena-Utilities.hpp in
	//	order to set up symbol visibility properly when building the actual
	//	library binary. This is only relevant for source files.

#include <Lucena-Utilities/Lucena-Utilities.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	NOP.
*/

bool Finalize()
{
	return true;
}


/*------------------------------------------------------------------------------
	NOP.
*/

bool Initialize()
{
	//	Identify the results of the feature tests if we have been asked to.
	//	SEEME Note that we can’t use preprocessor or template magic here since
	//	the objective is to display compile-time warnings (as opposed to
	//	run-time warnings or compile-time errors). If #warning allowed macro
	//	substitution, we’d be in luck, but unfortunately, that feature is an
	//	MSVC extension, only.
	#ifdef LUL_DIAGNOSTIC_feature_detection
		#undef LUL_DIAGNOSTIC_feature_detection
		#define LUL_DIAGNOSTIC_feature_detection 0
	#endif

	#if LUL_DIAGNOSTIC_feature_detection
		#if LUL_CPP98_EXCEPTIONS
			#warning "LUL_CPP98_EXCEPTIONS supported"
		#else
			#warning "LUL_CPP98_EXCEPTIONS not supported"
		#endif

		#if LUL_CPP98_RTTI
			#warning "LUL_CPP98_RTTI supported"
		#else
			#warning "LUL_CPP98_RTTI not supported"
		#endif

		#if LUL_CPP11_MINIMAL_GARBAGE_COLLECTION
			#warning "LUL_CPP11_MINIMAL_GARBAGE_COLLECTION supported"
		#else
			#warning "LUL_CPP11_MINIMAL_GARBAGE_COLLECTION not supported"
		#endif

		#if LUL_CPP11_THREADSAFE_STATIC_INIT
			#warning "LUL_CPP11_THREADSAFE_STATIC_INIT supported"
		#else
			#warning "LUL_CPP11_THREADSAFE_STATIC_INIT not supported"
		#endif

		#if LUL_CPP14_SIZED_DEALLOCATION
			#warning "LUL_CPP14_SIZED_DEALLOCATION supported"
		#else
			#warning "LUL_CPP14_SIZED_DEALLOCATION not supported"
		#endif

		#if LUL_CPP17_TEMPLATE_TEMPLATE_ARGS
			#warning "LUL_CPP17_TEMPLATE_TEMPLATE_ARGS supported"
		#else
			#warning "LUL_CPP17_TEMPLATE_TEMPLATE_ARGS not supported"
		#endif

		#if LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS
			#warning "LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS supported"
		#else
			#warning "LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS
			#warning "LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_LIKELY
			#warning "LUL_CPP2A_ATTRIBUTE_LIKELY supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_LIKELY not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_UNLIKELY
			#warning "LUL_CPP2A_ATTRIBUTE_UNLIKELY supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_UNLIKELY not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY
			#warning "LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY not supported"
		#endif

		#if LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS
			#warning "LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS supported"
		#else
			#warning "LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS not supported"
		#endif

		#if LUL_CPP2A_CONCEPTS
			#warning "LUL_CPP2A_CONCEPTS supported"
		#else
			#warning "LUL_CPP2A_CONCEPTS not supported"
		#endif

		#if LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS
			#warning "LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS supported"
		#else
			#warning "LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS not supported"
		#endif

		#if LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION
			#warning "LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION supported"
		#else
			#warning "LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_ASSERT
			#warning "LUL_CPP2A_ATTRIBUTE_ASSERT supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_ASSERT not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_ENSURES
			#warning "LUL_CPP2A_ATTRIBUTE_ENSURES supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_ENSURES not supported"
		#endif

		#if LUL_CPP2A_ATTRIBUTE_EXPECTS
			#warning "LUL_CPP2A_ATTRIBUTE_EXPECTS supported"
		#else
			#warning "LUL_CPP2A_ATTRIBUTE_EXPECTS not supported"
		#endif

		#if LUL_CPP2A_CONTRACTS
			#warning "LUL_CPP2A_CONTRACTS supported"
		#else
			#warning "LUL_CPP2A_CONTRACTS not supported"
		#endif

		#if LUL_CPP2A_COROUTINES
			#warning "LUL_CPP2A_COROUTINES supported"
		#else
			#warning "LUL_CPP2A_COROUTINES not supported"
		#endif

		#if LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS
			#warning "LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS supported"
		#else
			#warning "LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS not supported"
		#endif

		#if LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS
			#warning "LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS supported"
		#else
			#warning "LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS not supported"
		#endif

		#if LUL_CPP2A_DESIGNATED_INITIALIZERS
			#warning "LUL_CPP2A_DESIGNATED_INITIALIZERS supported"
		#else
			#warning "LUL_CPP2A_DESIGNATED_INITIALIZERS not supported"
		#endif

		#if LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES
			#warning "LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES supported"
		#else
			#warning "LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES not supported"
		#endif

		#if LUL_CPP2A_EXPLICIT_BOOL
			#warning "LUL_CPP2A_EXPLICIT_BOOL supported"
		#else
			#warning "LUL_CPP2A_EXPLICIT_BOOL not supported"
		#endif

		#if LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR
			#warning "LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR supported"
		#else
			#warning "LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR not supported"
		#endif

		#if LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION
			#warning "LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION supported"
		#else
			#warning "LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION not supported"
		#endif

		#if LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS
			#warning "LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS supported"
		#else
			#warning "LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS not supported"
		#endif

		#if LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE
			#warning "LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE supported"
		#else
			#warning "LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE not supported"
		#endif

		#if LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR
			#warning "LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR supported"
		#else
			#warning "LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR not supported"
		#endif

		#if LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS
			#warning "LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS supported"
		#else
			#warning "LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS not supported"
		#endif

		#if LUL_CPP2A_TYPENAME_OPTIONAL
			#warning "LUL_CPP2A_TYPENAME_OPTIONAL supported"
		#else
			#warning "LUL_CPP2A_TYPENAME_OPTIONAL not supported"
		#endif

		#if LUL_CPP2A_VA_OPT
			#warning "LUL_CPP2A_VA_OPT supported"
		#else
			#warning "LUL_CPP2A_VA_OPT not supported"
		#endif

		#if LUL_C99_PREPROCESSOR
			#warning "LUL_C99_PREPROCESSOR supported"
		#else
			#warning "LUL_C99_PREPROCESSOR not supported"
		#endif

		#if LUL_LIBCPP17_ANY
			#warning "LUL_LIBCPP17_ANY supported"
		#else
			#warning "LUL_LIBCPP17_ANY not supported"
		#endif

		#if LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS
			#warning "LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS supported"
		#else
			#warning "LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS not supported"
		#endif

		#if LUL_LIBCPP17_EXECUTION
			#warning "LUL_LIBCPP17_EXECUTION supported"
		#else
			#warning "LUL_LIBCPP17_EXECUTION not supported"
		#endif

		#if LUL_LIBCPP17_FILESYSTEM
			#warning "LUL_LIBCPP17_FILESYSTEM supported"
		#else
			#warning "LUL_LIBCPP17_FILESYSTEM not supported"
		#endif

		#if LUL_LIBCPP17_FILESYSTEM_EXP
			#warning "LUL_LIBCPP17_FILESYSTEM_EXP supported"
		#else
			#warning "LUL_LIBCPP17_FILESYSTEM_EXP not supported"
		#endif

		#if LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE
			#warning "LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE supported"
		#else
			#warning "LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE not supported"
		#endif

		#if LUL_LIBCPP17_LAUNDER
			#warning "LUL_LIBCPP17_LAUNDER supported"
		#else
			#warning "LUL_LIBCPP17_LAUNDER not supported"
		#endif

		#if LUL_LIBCPP17_OPTIONAL
			#warning "LUL_LIBCPP17_OPTIONAL supported"
		#else
			#warning "LUL_LIBCPP17_OPTIONAL not supported"
		#endif

		#if LUL_LIBCPP17_PARALLEL_ALGORITHM
			#warning "LUL_LIBCPP17_PARALLEL_ALGORITHM supported"
		#else
			#warning "LUL_LIBCPP17_PARALLEL_ALGORITHM not supported"
		#endif

		#if LUL_LIBCPP17_SPLICING_MAPS_AND_SETS
			#warning "LUL_LIBCPP17_SPLICING_MAPS_AND_SETS supported"
		#else
			#warning "LUL_LIBCPP17_SPLICING_MAPS_AND_SETS not supported"
		#endif

		#if LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS
			#warning "LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS supported"
		#else
			#warning "LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS not supported"
		#endif

		#if LUL_LIBCPP17_VARIANT
			#warning "LUL_LIBCPP17_VARIANT supported"
		#else
			#warning "LUL_LIBCPP17_VARIANT not supported"
		#endif

		#if LUL_LIBCPP2A_BIT_CAST
			#warning "LUL_LIBCPP2A_BIT_CAST supported"
		#else
			#warning "LUL_LIBCPP2A_BIT_CAST not supported"
		#endif

		#if LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE
			#warning "LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE supported"
		#else
			#warning "LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE not supported"
		#endif

		#if LUL_LIBCPP2A_CONCEPT_LIBRARY
			#warning "LUL_LIBCPP2A_CONCEPT_LIBRARY supported"
		#else
			#warning "LUL_LIBCPP2A_CONCEPT_LIBRARY not supported"
		#endif

		#if LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
			#warning "LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP supported"
		#else
			#warning "LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP not supported"
		#endif

		#if LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY
			#warning "LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY supported"
		#else
			#warning "LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY not supported"
		#endif

		#if LUL_LIBCPP2A_COROUTINES
			#warning "LUL_LIBCPP2A_COROUTINES supported"
		#else
			#warning "LUL_LIBCPP2A_COROUTINES not supported"
		#endif

		#if LUL_LIBCPP2A_COROUTINES_EXP
			#warning "LUL_LIBCPP2A_COROUTINES_EXP supported"
		#else
			#warning "LUL_LIBCPP2A_COROUTINES_EXP not supported"
		#endif

		#if LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS
			#warning "LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS supported"
		#else
			#warning "LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS not supported"
		#endif

		#if LUL_LIBCPP2A_FLOATING_POINT_ATOMIC
			#warning "LUL_LIBCPP2A_FLOATING_POINT_ATOMIC supported"
		#else
			#warning "LUL_LIBCPP2A_FLOATING_POINT_ATOMIC not supported"
		#endif

		#if LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE
			#warning "LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE supported"
		#else
			#warning "LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE not supported"
		#endif

		#if LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX
			#warning "LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX supported"
		#else
			#warning "LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX not supported"
		#endif

		#if LUL_LIBCPP2A_SPAN
			#warning "LUL_LIBCPP2A_SPAN supported"
		#else
			#warning "LUL_LIBCPP2A_SPAN not supported"
		#endif

		#if LUL_LIBCPP2A_STD_ATOMIC_REF
			#warning "LUL_LIBCPP2A_STD_ATOMIC_REF supported"
		#else
			#warning "LUL_LIBCPP2A_STD_ATOMIC_REF not supported"
		#endif

		#if LUL_LIBCPP2A_STD_ENDIAN
			#warning "LUL_LIBCPP2A_STD_ENDIAN supported"
		#else
			#warning "LUL_LIBCPP2A_STD_ENDIAN not supported"
		#endif

		#if LUL_LIBCPP2A_STD_REMOVE_CVREF
			#warning "LUL_LIBCPP2A_STD_REMOVE_CVREF supported"
		#else
			#warning "LUL_LIBCPP2A_STD_REMOVE_CVREF not supported"
		#endif

		#if LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING
			#warning "LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING supported"
		#else
			#warning "LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING not supported"
		#endif

		#if LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM
			#warning "LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM supported"
		#else
			#warning "LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM not supported"
		#endif

		#if LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE
			#warning "LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE supported"
		#else
			#warning "LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE not supported"
		#endif

		#if LUL_LIBCPP2A_TYPE_IDENTITY
			#warning "LUL_LIBCPP2A_TYPE_IDENTITY supported"
		#else
			#warning "LUL_LIBCPP2A_TYPE_IDENTITY not supported"
		#endif

		#if LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER
			#warning "LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER supported"
		#else
			#warning "LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER not supported"
		#endif

		#if LUL_LIBCPP2A_VERSION
			#warning "LUL_LIBCPP2A_VERSION supported"
		#else
			#warning "LUL_LIBCPP2A_VERSION not supported"
		#endif
	#endif	//	LUL_DIAGNOSTIC_feature_detection

	return true;
}


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace

#endif	//	LUL_CONFIG_headers_only
