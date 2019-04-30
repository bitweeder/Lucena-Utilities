/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“CompilerSetup.hpp”
	Copyright © 2007-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	We assume support for at least C++17. See lulFeatureSetup.hpp for
	descriptions of the various macros and flags.

------------------------------------------------------------------------------*/


#pragma once


//	std
//	SEEME We always guard inclusion of system headers when using MSVS due to
//	their noisiness at high warning levels.
#if defined (_MSC_VER) && defined (_WIN32)
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
	backs.
*/

#if defined (LUL_NAME_COMPILER)
	#error "Don’t define LUL_NAME_COMPILER outside of this file."
#endif	//	LUL_NAME_COMPILER

#if defined (LUL_TARGET_COMPILER_CLANG) || \
	defined (LUL_TARGET_COMPILER_GCC) || \
	defined (LUL_TARGET_COMPILER_MSVC)

	#error "Don’t define LUL_TARGET_COMPILER_xxx outside of this file."
#endif	//	LUL_TARGET_COMPILER check

#if defined (LUL_TARGET_CPU_X86) || \
	defined (LUL_TARGET_CPU_X86_64) || \
	defined (LUL_TARGET_CPU_IA64) || \
	defined (LUL_TARGET_CPU_ARM) || \
	defined (LUL_TARGET_CPU_ARM_64)

	#error "Don’t define LUL_TARGET_CPU_xxx outside of this file."
#endif	//	LUL_TARGET_CPU check

#if defined (LUL_TARGET_CPU_X86_FAMILY) or \
	defined (LUL_TARGET_CPU_ARM_FAMILY)

	#error "Don’t define LUL_TARGET_CPU_xxx_FAMILY outside of this file."
#endif	//	LUL_TARGET_CPU_FAMILY check

#if defined (LUL_TARGET_VEC_SSE) or \
	defined (LUL_TARGET_VEC_SSE2) or \
	defined (LUL_TARGET_VEC_SSE3) or \
	defined (LUL_TARGET_VEC_SSE41) or \
	defined (LUL_TARGET_VEC_SSE42) or \
	defined (LUL_TARGET_VEC_AVX) or \
	defined (LUL_TARGET_VEC_AVX2)

	#error "Don’t define LUL_TARGET_VEC_xxx outside of this file."
#endif	//	LUL_TARGET_VEC check

#if defined (LUL_TARGET_RT_LITTLE_ENDIAN) || \
	defined (LUL_TARGET_RT_BIG_ENDIAN) || \
	defined (LUL_TARGET_RT_32_BIT) || \
	defined (LUL_TARGET_RT_64_BIT) || \
	defined (LUL_TARGET_RT_COFF) || \
	defined (LUL_TARGET_RT_ELF) || \
	defined (LUL_TARGET_RT_MACHO) || \
	defined (LUL_TARGET_RT_WASM)

	#error "Don’t define LUL_TARGET_RT_xxx outside of this file."
#endif	//	LUL_TARGET_RT check

#if defined (LUL_TYPE_HAS_INT64) || \
	defined (LUL_TYPE_HAS_INT128) || \
	defined (LUL_TYPE_EXACT_WIDTH_INTEGERS) || \
	defined (LUL_TYPE_DOUBLE_GT_FLOAT) || \
	defined (LUL_TYPE_LONG_DOUBLE_GT_DOUBLE) || \
	defined (LUL_TYPE_WCHAR_T_IS_16_BITS)

	#error "Don’t define LUL_TYPE_xxx outside of this file."
#endif	//	LUL_TYPE check

#if defined (LUL_CPP98_EXCEPTIONS) || \
	defined (LUL_CPP98_RTTI)

	#error "Don’t define LUL_CPP98_xxx outside of this file."
#endif	//	LUL_CPP98 check

#if defined (LUL_CPP11_MINIMAL_GARBAGE_COLLECTION) || \
	defined (LUL_CPP11_THREADSAFE_STATIC_INIT)

	#error "Don’t define LUL_CPP11_xxx outside of this file."
#endif	//	LUL_CPP11 check

#if defined (LUL_CPP14_SIZED_DEALLOCATION)
	#error "Don’t define LUL_CPP14_xxx outside of this file."
#endif	//	LUL_CPP14 check

#if defined (LUL_CPP17_TEMPLATE_TEMPLATE_ARGS)
	#error "Don’t define LUL_CPP17_xxx outside of this file."
#endif	//	LUL_CPP17 check

#if defined (LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS) || \
	defined (LUL_CPP2A_ATTRIBUTE_ASSERT) || \
	defined (LUL_CPP2A_ATTRIBUTE_ENSURES) || \
	defined (LUL_CPP2A_ATTRIBUTE_EXPECTS) || \
	defined (LUL_CPP2A_ATTRIBUTE_LIKELY) || \
	defined (LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS) || \
	defined (LUL_CPP2A_ATTRIBUTE_UNLIKELY) || \
	defined (LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY) || \
	defined (LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS) || \
	defined (LUL_CPP2A_CONCEPTS) || \
	defined (LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS) || \
	defined (LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION) || \
	defined (LUL_CPP2A_CONTRACTS) || \
	defined (LUL_CPP2A_COROUTINES) || \
	defined (LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS) || \
	defined (LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS) || \
	defined (LUL_CPP2A_DESIGNATED_INITIALIZERS) || \
	defined (LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES) || \
	defined (LUL_CPP2A_EXPLICIT_BOOL) || \
	defined (LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR) || \
	defined (LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION) || \
	defined (LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS) || \
	defined (LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE) || \
	defined (LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR) || \
	defined (LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS) || \
	defined (LUL_CPP2A_TYPENAME_OPTIONAL) || \
	defined (LUL_CPP2A_VA_OPT)

	#error "Don’t define LUL_CPP2A_xxx outside of this file."
#endif	//	LUL_CPP2A check

#if defined (LUL_C99_PREPROCESSOR)
	#error "Don’t define LUL_C99_xxx outside of this file."
#endif	//	LUL_C99 check

#if defined (LUL_CLASS_FORCE_EBCO)
	#error "Don’t define LUL_CLASS_xxx outside of this file."
#endif	//	LUL_CLASS_xxx check

#if defined (LUL_FUNC_CALL_C) || \
	defined (LUL_FUNC_CALL_STD) || \
	defined (LUL_FUNC_CALLBACK_C) || \
	defined (LUL_FUNC_CALLBACK_STD)

	#error "Don’t define LUL_FUNC_xxx outside of this file."
#endif	//	LUL_FUNC_xxx check

#if defined (LUL_BUILTIN_likely) || \
	defined (LUL_BUILTIN_unlikely)

	#error "Don’t define LUL_BUILTIN_xxx outside of this file."
#endif	//	LUL_BUILTIN_xxx check


/*------------------------------------------------------------------------------
	Compiler Macros

	SEEME Arguably, it would be more desirable to give each compiler its own
	#include file and include them here conditionally rather than go the
	monolithic route. We trade off potential unnecessary recompilation of
	unaffected projects for simplified maintenance at this time.
*/

#if defined (__clang__) && defined (__llvm__) && defined (__APPLE_CC__)
	//	clang-llvm compiler targeting Apple platforms

	//	Note that Apple has repurposed the __clang_major__, __clang_minor__,
	//	and __clang_patchlevel__ macros, so they are no longer in line with
	//	other clang distributions. We can test whether we’re in Apple clang by
	//	looking for __APPLE_CC__, __apple_build_version__, or some other macro.
	//	SEEME Apple LLVM 9.1 is the first release that actually advertises
	//	C++17 support (as per __cplusplus); previously, it only had C++14 with
	//	almost all C++17 extensions. Note that libc++ is still broken due to
	//	runtime issues, as described below, as well as other things.
	#if (__clang_major__ < 10)
		#error "Settings are only valid for Apple LLVM 10.0+"
	#else
		//	Tested with a minimum of Xcode 10.0; the bundled compiler and
		//	Standard Library are roughly compatible with clang 6 and libc++,
		//	while adding some Apple-proprietary stuff.
		//	SEEME We’re actually compatible with LLVM 9.4+, mostly, but we
		//	don’t test with it any more since various non-compiler defects with
		//	regard to C++17 make it an annoyance to support.
	#endif

	#if __cplusplus < 201703L
		#error "std=c++17 or higher is required"
	#endif

	//	Identify processor and characteristics.
	//	Note that AMD64 and x86_64 both trigger each other’s identifiers. We
	//	simply refer to all of them as LUL_TARGET_CPU_X86_64.
	#if __x86_64__ or __amd64__
		#define LUL_TARGET_CPU_X86_64				1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif __i386__
		#define LUL_TARGET_CPU_X86					1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif __aarch64__
		#define LUL_TARGET_CPU_ARM_64				1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#elif __arm__
		#define LUL_TARGET_CPU_ARM					1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#else
		#error "Processor unknown. Update lulCompilerSetup.hpp."
	#endif

	#if __LITTLE_ENDIAN__
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#elif __BIG_ENDIAN__
		#define LUL_TARGET_RT_BIG_ENDIAN			1
	#endif

	#if __LP64__
		#define LUL_TARGET_RT_64_BIT				1
	#else
		#define LUL_TARGET_RT_32_BIT				1
	#endif

	#if __SIZEOF_DOUBLE__ > __SIZEOF_FLOAT__
		#define LUL_TYPE_DOUBLE_GT_FLOAT			1
	#endif

	#if __SIZEOF_LONG_DOUBLE__ > __SIZEOF_DOUBLE__
		#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		1
	#endif


	//	Identify vector instruction sets, if any.
	#ifdef __SSE__
		#define LUL_TARGET_VEC_SSE					1
	#endif

	#ifdef __SSE2__
		#define LUL_TARGET_VEC_SSE2					1
	#endif

	#ifdef __SSE3__
		#define LUL_TARGET_VEC_SSE3					1
	#endif

	#ifdef __SSE4_1__
		#define LUL_TARGET_VEC_SSE41				1
	#endif

	#ifdef __SSE4_2__
		#define LUL_TARGET_VEC_SSE42				1
	#endif

	#ifdef __AVX__
		#define LUL_TARGET_VEC_AVX					1
	#endif

	#ifdef __AVX2__
		#define LUL_TARGET_VEC_AVX2					1
	#endif

	//	Currently unused
	#ifdef __AVX512BW__
	#endif

	#ifdef __AVX512CD__
	#endif

	#ifdef __AVX512DQ__
	#endif

	#ifdef __AVX512F__
	#endif

	#ifdef __AVX5124FMAPS__
	#endif

	#ifdef __AVX5124VNNIW__
	#endif

	#ifdef __AVX512IFMA__
	#endif

	#ifdef __AVX512VBMI__
	#endif

	#ifdef __AVX512VPOPCNTDQ__
	#endif

	#ifdef __AVX512VL__
	#endif


	//	Identify executable file fromat.
	#ifdef __MACH__
		#define LUL_TARGET_RT_MACHO					1
	#else
		#error "No other executable file formats supported for Apple targets"
	#endif


	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1


	//	SEEME Apparently, __STDC_VERSION__ only gets set if we’re using the C
	//	compiler, so this test doesn’t work. Since we’re requiring C++17 or
	//	greater, we assume the following C99 features are available.
	#define LUL_C99_PREPROCESSOR					1


	//	Test for C++98 features.
	//	These can be conditionally disabled.
	//	SEEME Note that we can’t rely on the SD-6 macros for these since
	//	they’ve been removed for standardization.
	#if __has_feature(cxx_exceptions)
		#define LUL_CPP98_EXCEPTIONS										199711L
	#endif

	#if __has_feature(cxx_rtti)
		#define LUL_CPP98_RTTI												199711L
	#endif

	//	Test for C++11 features.
	//	These can be conditionally disabled.
	#if __cpp_threadsafe_static_init
		#define LUL_CPP11_THREADSAFE_STATIC_INIT							__cpp_threadsafe_static_init
	#endif

	//	Test for C++14 features.
	//	As of vanilla clang 8, this is disabled by default in order to avoid
	//	ABI-breaking changes. It’s unclear what sign from heaven will finally
	//	drive the maintainers to enable it.
	#if __cpp_sized_deallocation
		#define LUL_CPP14_SIZED_DEALLOCATION								__cpp_sized_deallocation
	#endif

	//	Test for C++17 features.
	//	SEEME Apple continues to lag vanilla clang by over a year in odd ways.
	//	As of Apple LLVM 10.0, their clang compiler advertises full C++17
	//	compliance, but a number of feature test macros fail (and then there’s
	//	the real Standard Library deficiencies...):
	//
	//		- __cpp_guaranteed_copy_elision is undefined; testing suggests
	//			the feature is implemented; no tracking
	//		- __cpp_deduction_guides returns an outdated value; no tracking,
	//			as it’s unclear if this needs it
	//		- __cpp_nontype_template_parameter_auto is undefined; testing
	//			suggests the feature is implemented; no tracking
	//
	//	As for other potential issues, __cpp_threadsafe_static_init -can- be
	//	set to 0 depending on compiler arguments, but defaults to being on.

	//	As of vanilla clang 8, this is disabled by default in order to avoid a
	//	defect introduced by the associated proposal (which was intended to
	//	resolve another defect).
	#if __cpp_template_template_args
		#define LUL_CPP17_TEMPLATE_TEMPLATE_ARGS							__cpp_template_template_args
	#endif

	//	Test for C++2a features.
	//	SEEME There is partial support for this going back years, but it’s not
	//	fully in-line with the accepted proposal. No ETA for compliance.
//	#define LUL_CPP2A_DESIGNATED_INITIALIZERS								1L

	//	SEEME This may not be fully compliant, as the proposal values may not
	//	be what actually gets returned by various macros. Untested.
	#define LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS					1L

	//	FIXME Speculative; Xcode 10 seems to be tracking vanilla clang 6, but
	//	these are untested.
	#if (__clang_major__ >= 10)
		#define LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS					1L
		#define LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS			1L
		#define LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS		1L
		#define LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION	1L
		#define LUL_CPP2A_VA_OPT											1L
	#endif

	#if __cpp_concepts
		#define LUL_CPP2A_CONCEPTS											__cpp_concepts
	#endif

	#if __cpp_coroutines
		#define LUL_CPP2A_COROUTINES										__cpp_coroutines
	#endif

	#if __cpp_explicit_bool
		#define LUL_CPP2A_EXPLICIT_BOOL										__cpp_explicit_bool
	#endif

	#if __cpp_nontype_template_parameter_class
		#define LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS		__cpp_nontype_template_parameter_class
	#endif

	#define LUL_CPP2A_ATTRIBUTE_LIKELY										__has_cpp_attribute (likely)
	#define LUL_CPP2A_ATTRIBUTE_UNLIKELY									__has_cpp_attribute (unlikely)
	#define LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY						(LUL_CPP2A_ATTRIBUTE_LIKELY && LUL_CPP2A_ATTRIBUTE_UNLIKELY)

	#define LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS							__has_cpp_attribute (no_unique_address)

	#define LUL_CPP2A_ATTRIBUTE_ASSERT										__has_cpp_attribute (assert)
	#define LUL_CPP2A_ATTRIBUTE_ENSURES										__has_cpp_attribute (ensures)
	#define LUL_CPP2A_ATTRIBUTE_EXPECTS										__has_cpp_attribute (expects)
	#define LUL_CPP2A_CONTRACTS												(LUL_CPP2A_ATTRIBUTE_ASSERT && LUL_CPP2A_ATTRIBUTE_ENSURES && LUL_CPP2A_ATTRIBUTE_EXPECTS)


	//	intrinsic functions
	#define LUL_BUILTIN_likely(LUL_expr_)			__builtin_expect (LUL_expr_, true)
	#define LUL_BUILTIN_unlikely(LUL_expr_)			__builtin_expect (LUL_expr_, false)


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_CLASS_EXPORT					__attribute__ ((__visibility__("default")))
	#define LUL_VIS_CLASS_IMPORT					__attribute__ ((__visibility__("default")))

	#if __has_attribute (__type_visibility__)
		#define LUL_VIS_ENUM						__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT	__attribute__ ((__visibility__("default")))
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT	__attribute__ ((__visibility__("default")))
	#else
		#define LUL_VIS_ENUM

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT
	#endif

	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_EXPORT
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_IMPORT

	#define LUL_VIS_MEMBER_CLASS_TEMPLATE			__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_MEMBER_FUNCTION_TEMPLATE		inline __attribute__ ((__visibility__("hidden")))
		//	SEEME The “inline” declaration is extraneous, but required due to
		//	 a currently unaddressed clang defect (cf., the elusive PR32114).

	#define LUL_VIS_FUNC_EXPORT						__attribute__ ((__visibility__("default")))
	#define LUL_VIS_FUNC_IMPORT						__attribute__ ((__visibility__("default")))

	//	SEEME The first definition is a fallback if it turns out I’ve
	//	misunderstood the requirements on general inline functions.
//	#define LUL_VIS_INLINE_FUNC						LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC
	#define LUL_VIS_INLINE_FUNC						__attribute__ ((__visibility__("hidden")))

	//	SEEME Using the internal_linkage attribute is equivalent to using the
	//	static keyword in C, and is semantically preferrable to inlining.
	//	However, what we -really- want is be able to strip this down to just
	//	LUL_VIS_HIDDEN, but this requires linker support we don’t have, yet.
	//	See <http://lists.llvm.org/pipermail/cfe-dev/2018-July/058450.html> and
	//	<https://reviews.llvm.org/D49240?id=155185> for details.
	#if __has_attribute (internal_linkage)
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), internal_linkage))
	#else
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), __always_inline__))
	#endif

	#define LUL_VIS_OVERLOADABLE_FUNC_EXPORT		__attribute__ ((__visibility__("default")))
	#define LUL_VIS_OVERLOADABLE_FUNC_IMPORT		__attribute__ ((__visibility__("default")))

	#define LUL_VIS_EXTERN_EXPORT
	#define LUL_VIS_EXTERN_IMPORT

	#define LUL_VIS_EXCEPTION_EXPORT				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_IMPORT				__attribute__ ((__visibility__("default")))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


	//	Set up identifiers
	#define LUL_NAME_COMPILER u8"clang version " 	__clang_version__
	#define LUL_TARGET_COMPILER_CLANG				1

#elif defined (__clang__) && defined (__llvm__)
	//	generic clang-llvm compiler

	#if (__clang_major__ < 6)
		#error "Settings are only valid for clang 6.0+"
	#else
		//	FIXME This is actually untested, as it’s just a fix-up of the
		//	Apple LLVM section.
	#endif

	#if __cplusplus < 201703L
		#error "std=c++17 or higher is required"
	#endif

	//	Identify processor and characteristics.
	//	Note that AMD64 and x86_64 both trigger each other’s identifiers. We
	//	simply refer to all of them as LUL_TARGET_CPU_X86_64.
	#if __x86_64__ or __amd64__
		#define LUL_TARGET_CPU_X86_64				1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif __i386__
		#define LUL_TARGET_CPU_X86					1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif __aarch64__
		#define LUL_TARGET_CPU_ARM_64				1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#elif __arm__
		#define LUL_TARGET_CPU_ARM					1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#else
		#error "Processor unknown. Update lulCompilerSetup.hpp."
	#endif

	#if __LITTLE_ENDIAN__
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#elif __BIG_ENDIAN__
		#define LUL_TARGET_RT_BIG_ENDIAN			1
	#endif

	#if __LP64__
		#define LUL_TARGET_RT_64_BIT				1
	#else
		#define LUL_TARGET_RT_32_BIT				1
	#endif

	#if __SIZEOF_DOUBLE__ > __SIZEOF_FLOAT__
		#define LUL_TYPE_DOUBLE_GT_FLOAT			1
	#endif

	#if __SIZEOF_LONG_DOUBLE__ > __SIZEOF_DOUBLE__
		#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		1
	#endif


	//	Identify vector instruction sets, if any.
	#ifdef __SSE__
		#define LUL_TARGET_VEC_SSE				1
	#endif

	#ifdef __SSE2__
		#define LUL_TARGET_VEC_SSE2				1
	#endif

	#ifdef __SSE3__
		#define LUL_TARGET_VEC_SSE3				1
	#endif

	#ifdef __SSE4_1__
		#define LUL_TARGET_VEC_SSE41			1
	#endif

	#ifdef __SSE4_2__
		#define LUL_TARGET_VEC_SSE42			1
	#endif

	#ifdef __AVX__
		#define LUL_TARGET_VEC_AVX				1
	#endif

	#ifdef __AVX2__
		#define LUL_TARGET_VEC_AVX2				1
	#endif


	//	Identify executable file fromat
	#if defined (__ELF__)
		#define LUL_TARGET_RT_ELF					1
	#elif defined (_WIN32)
		#define LUL_TARGET_RT_COFF					1
	#elif defined (__MACH__)
		#define LUL_TARGET_RT_MACHO					1
	#elif defined (__wasm__)
		#define LUL_TARGET_RT_WASM					1
	#else
		#error "Unknown executable file format"
	#endif


	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1


	//	SEEME Apparently, __STDC_VERSION__ only gets set if we’re using the C
	//	compiler, so this test doesn’t work. Since we’re requiring C++14 or
	//	greater, we assume the following C99 features are available.
	#if 1	//	__STDC_VERSION__ >= 199901L
		#define LUL_C99_PREPROCESSOR				1
	#else
		#warning "D'oh!"
	#endif	//	__STDC_VERSION__


	//	Test for C++98 features.
	//	These can be conditionally disabled.
	//	SEEME Note that we can’t rely on the SD-6 macros for these since
	//	they’ve been removed for standardization.
	#if __has_feature(cxx_exceptions)
		#define LUL_CPP98_EXCEPTIONS										199711L
	#endif

	#if __has_feature(cxx_rtti)
		#define LUL_CPP98_RTTI												199711L
	#endif

	//	Test for C++11 features.
	//	These can be conditionally disabled.
	#if __cpp_threadsafe_static_init
		#define LUL_CPP11_THREADSAFE_STATIC_INIT							__cpp_threadsafe_static_init
	#endif

	//	Test for C++14 features.
	//	As of clang 8, this is disabled by default in order to avoid ABI-
	//	breaking changes. It’s unclear what sign from heaven will finally
	//	drive the maintainers to enable it.
	#if __cpp_sized_deallocation
		#define LUL_CPP14_SIZED_DEALLOCATION								__cpp_sized_deallocation
	#endif

	//	Test for C++17 features.
	//	As of clang 8, this is disabled by default in order to avoid a defect
	//	introduced by the associated proposal (which was intended to resolve
	//	another defect).
	#if __cpp_template_template_args
		#define LUL_CPP17_TEMPLATE_TEMPLATE_ARGS							__cpp_template_template_args
	#endif

	//	Test for C++2a features.
	//	SEEME There is partial support for this going back years, but it’s not
	//	fully in-line with the accepted proposal. No ETA for compliance.
//	#define LUL_CPP2A_DESIGNATED_INITIALIZERS								1L

	//	SEEME This may not be fully compliant, as the proposal values may not
	//	be what actually gets returned by vrious macros. Untested.
	#define LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS					1L

	#if (__clang_major__ >= 6)
		#define LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS					1L
		#define LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS			1L
		#define LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS		1L
		#define LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION	1L
		#define LUL_CPP2A_VA_OPT											1L
	#endif

	#if __cpp_concepts
		#define LUL_CPP2A_CONCEPTS											__cpp_concepts
	#endif

	#if __cpp_coroutines
		#define LUL_CPP2A_COROUTINES										__cpp_coroutines
	#endif

	#if __cpp_explicit_bool
		#define LUL_CPP2A_EXPLICIT_BOOL										__cpp_explicit_bool
	#endif

	#if __cpp_nontype_template_parameter_class
		#define LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS		__cpp_nontype_template_parameter_class
	#endif

	#define LUL_CPP2A_ATTRIBUTE_LIKELY										__has_cpp_attribute (likely)
	#define LUL_CPP2A_ATTRIBUTE_UNLIKELY									__has_cpp_attribute (unlikely)
	#define LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY						(LUL_CPP2A_ATTRIBUTE_LIKELY && LUL_CPP2A_ATTRIBUTE_UNLIKELY)

	#define LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS							__has_cpp_attribute (no_unique_address)

	#define LUL_CPP2A_ATTRIBUTE_ASSERT										__has_cpp_attribute (assert)
	#define LUL_CPP2A_ATTRIBUTE_ENSURES										__has_cpp_attribute (ensures)
	#define LUL_CPP2A_ATTRIBUTE_EXPECTS										__has_cpp_attribute (expects)
	#define LUL_CPP2A_CONTRACTS												(LUL_CPP2A_ATTRIBUTE_ASSERT && LUL_CPP2A_ATTRIBUTE_ENSURES && LUL_CPP2A_ATTRIBUTE_EXPECTS)


	//	intrinsic functions
	#define LUL_BUILTIN_likely(LUL_expr_)			__builtin_expect (LUL_expr_, true)
	#define LUL_BUILTIN_unlikely(LUL_expr_)			__builtin_expect (LUL_expr_, false)


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_CLASS_EXPORT					__attribute__ ((__visibility__("default")))
	#define LUL_VIS_CLASS_IMPORT					__attribute__ ((__visibility__("default")))

	#if __has_attribute (__type_visibility__)
		#define LUL_VIS_ENUM						__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT	__attribute__ ((__visibility__("default")))
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT	__attribute__ ((__visibility__("default")))
	#else
		#define LUL_VIS_ENUM

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT
	#endif

	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_EXPORT
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_IMPORT

	#define LUL_VIS_MEMBER_CLASS_TEMPLATE			__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_MEMBER_FUNCTION_TEMPLATE		inline __attribute__ ((__visibility__("hidden")))
		//	SEEME The “inline” declaration is extraneous, but required due to
		//	 a currently unaddressed clang defect (cf., the elusive PR32114).

	#define LUL_VIS_FUNC_EXPORT						__attribute__ ((__visibility__("default")))
	#define LUL_VIS_FUNC_IMPORT						__attribute__ ((__visibility__("default")))

	//	SEEME The first definition is a fallback if it turns out I’ve
	//	misunderstood the requirements on general inline functions.
//	#define LUL_VIS_INLINE_FUNC						LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC
	#define LUL_VIS_INLINE_FUNC						__attribute__ ((__visibility__("hidden")))

	//	SEEME Using the internal_linkage attribute is equivalent to using the
	//	static keyword in C, and is semantically preferrable to inlining.
	//	However, what we -really- want is be able to strip this down to just
	//	LUL_VIS_HIDDEN, but this requires linker support we don’t have, yet.
	//	See <http://lists.llvm.org/pipermail/cfe-dev/2018-July/058450.html> and
	//	<https://reviews.llvm.org/D49240?id=155185> for details.
	#if __has_attribute (internal_linkage)
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), internal_linkage))
	#else
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), __always_inline__))
	#endif

	#define LUL_VIS_OVERLOADABLE_FUNC_EXPORT		__attribute__ ((__visibility__("default")))
	#define LUL_VIS_OVERLOADABLE_FUNC_IMPORT		__attribute__ ((__visibility__("default")))

	#define LUL_VIS_EXTERN_EXPORT
	#define LUL_VIS_EXTERN_IMPORT

	#define LUL_VIS_EXCEPTION_EXPORT				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_IMPORT				__attribute__ ((__visibility__("default")))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


	//	Set up identifiers
	#define LUL_NAME_COMPILER u8"clang version " 	__clang_version__
	#define LUL_TARGET_COMPILER_CLANG				1

#elif defined (__GNUC__)
	//	generic gcc-based compiler
	//	Note that gcc guarantees that all system-specific predefined macros in
	//	the reserved namespace (i.e., __xxx__) will never resolve to 0 if they
	//	are actually defined.
	//	SEEME This must appear after clang, as the llvm derivatives have a
	//	tendency to define gcc’s macros; Apple LLVM, in particular, likes to
	//	pretend to be a crufty, ancient version of gcc.
	#if (__GNUC__ < 7) || ((__GNUC__ == 7) && (__GNUC_MINOR__ < 3))
		#error "Settings are only known to be valid for gcc 7.3+."
			//	For reference, 6.3 supports all C++11 and C++14 features except
			//	garbage collection. gcc 7 is required for many C++17 language
			//	features; note, though, that it does not support
			//	std::filesystem or the Parallelism TS, which are targeted for
			//	gcc 8.
	#endif

	#if __cplusplus < 201703L
		#error "std=c++17 or higher is required"
	#endif


	//	Identify processor.
	//	Note that AMD64 and x86_64 both trigger each other’s identifiers. We
	//	simply refer to all of them as LUL_TARGET_CPU_X86_64.
	#if defined (__x86_64__) or defined (__amd64__)
		#define LUL_TARGET_CPU_X86_64				1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif defined (__i386__)	//	 or defined (__i486__) or defined (__i586__) or defined (__i686__)
		#define LUL_TARGET_CPU_X86					1
		#define LUL_TARGET_CPU_X86_FAMILY			1
	#elif defined (__aarch64__)
		#define LUL_TARGET_CPU_ARM_64				1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#elif defined (__arm__)
		#define LUL_TARGET_CPU_ARM					1
		#define LUL_TARGET_CPU_ARM_FAMILY			1
	#else
		#error "Processor unknown. Update lulCompilerSetup.hpp."
	#endif

	#if __LITTLE_ENDIAN__
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#elif __BIG_ENDIAN__
		#define LUL_TARGET_RT_BIG_ENDIAN			1
	#endif

	#if __SIZEOF_DOUBLE__ > __SIZEOF_FLOAT__
		#define LUL_TYPE_DOUBLE_GT_FLOAT			1
	#endif

	#if __SIZEOF_LONG_DOUBLE__ > __SIZEOF_DOUBLE__
		#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		1
	#endif


	//	Identify vector instruction sets, if any.
	#ifdef __SSE__
		#define LUL_TARGET_VEC_SSE				1
	#endif

	#ifdef __SSE2__
		#define LUL_TARGET_VEC_SSE2				1
	#endif

	#ifdef __SSE3__
		#define LUL_TARGET_VEC_SSE3				1
	#endif

	#ifdef __SSE4_1__
		#define LUL_TARGET_VEC_SSE41			1
	#endif

	#ifdef __SSE4_2__
		#define LUL_TARGET_VEC_SSE42			1
	#endif

	#ifdef __AVX__
		#define LUL_TARGET_VEC_AVX				1
	#endif

	#ifdef __AVX2__
		#define LUL_TARGET_VEC_AVX2				1
	#endif


	//	Identify executable file fromat
	//	SEEME Architecture is the same across Linux derivatives (ignoring old
	//	a.out), but binary compatibility is not guaranteed.
	#if defined (__ELF__)
		#define LUL_TARGET_RT_ELF					1
	#elif defined (_WIN32)
		#define LUL_TARGET_RT_COFF					1
	#elif defined (__MACH__)
		#define LUL_TARGET_RT_MACHO					1
	#elif defined (__wasm__)
		#define LUL_TARGET_RT_WASM					1
	#else
		#error "Unknown executable file format"
	#endif


	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1

	#define LUL_C99_PREPROCESSOR					1


	//	Test for C++98 features.
	//	These can be conditionally disabled.
	//	SEEME Note that we can’t rely on the SD-6 macros for these since
	//	they’ve been removed for standardization.
	#if __EXCEPTIONS
		#define LUL_CPP98_EXCEPTIONS										199711L
	#endif

	#if __GXX_RTTI
		#define LUL_CPP98_RTTI												199711L
	#endif

	//	Test for C++11 features.
	//	These can be conditionally disabled.
	#if __cpp_threadsafe_static_init
		#define LUL_CPP11_THREADSAFE_STATIC_INIT							__cpp_threadsafe_static_init
	#endif

	//	Test for C++14 features.
	#if __cpp_sized_deallocation
		#define LUL_CPP14_SIZED_DEALLOCATION								__cpp_sized_deallocation
	#endif

	//	Test for C++17 features.
	#if __cpp_template_template_args
		#define LUL_CPP17_TEMPLATE_TEMPLATE_ARGS							__cpp_template_template_args
	#endif

	//	Test for C++2a features.
	//	SEEME This may not be fully compliant, as the proposal values may not
	//	be what actually gets returned by vrious macros. Untested.
	#define LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS					1L

	#if (__GNUC__ >= 8)
		#define LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS					1L
		#define LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS			1L
		#define LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS		1L
		#define LUL_CPP2A_DESIGNATED_INITIALIZERS							1L
		#define LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION	1L
		#define LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS		1L

		#define LUL_CPP2A_VA_OPT											1L
			//	SEEME This is arguably only a partial implementation since it
			//	fails for at least one corner case.
	#endif

	#if (__GNUC__ >= 9)
		#define LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS	1L
		#define LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR				1L
	#endif

	#if __cpp_concepts
		#define LUL_CPP2A_CONCEPTS											__cpp_concepts
	#endif

	#if __cpp_coroutines
		#define LUL_CPP2A_COROUTINES										__cpp_coroutines
	#endif

	#if __cpp_explicit_bool
		#define LUL_CPP2A_EXPLICIT_BOOL										__cpp_explicit_bool
	#endif

	#if __cpp_nontype_template_parameter_class
		#define LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS		__cpp_nontype_template_parameter_class
	#endif

	#define LUL_CPP2A_ATTRIBUTE_LIKELY										__has_cpp_attribute (likely)
	#define LUL_CPP2A_ATTRIBUTE_UNLIKELY									__has_cpp_attribute (unlikely)
	#define LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY						(LUL_CPP2A_ATTRIBUTE_LIKELY && LUL_CPP2A_ATTRIBUTE_UNLIKELY)

	#define LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS							__has_cpp_attribute (no_unique_address)

	#define LUL_CPP2A_ATTRIBUTE_ASSERT										__has_cpp_attribute (assert)
	#define LUL_CPP2A_ATTRIBUTE_ENSURES										__has_cpp_attribute (ensures)
	#define LUL_CPP2A_ATTRIBUTE_EXPECTS										__has_cpp_attribute (expects)
	#define LUL_CPP2A_CONTRACTS												(LUL_CPP2A_ATTRIBUTE_ASSERT && LUL_CPP2A_ATTRIBUTE_ENSURES && LUL_CPP2A_ATTRIBUTE_EXPECTS)


	//	intrinsic functions
	#define LUL_BUILTIN_likely(LUL_expr_)			__builtin_expect (LUL_expr_, true)
	#define LUL_BUILTIN_unlikely(LUL_expr_)			__builtin_expect (LUL_expr_, false)


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_CLASS_EXPORT					__attribute__ ((__visibility__("default")))
	#define LUL_VIS_CLASS_IMPORT					__attribute__ ((__visibility__("default")))

	#if __has_attribute (__type_visibility__)
		#define LUL_VIS_ENUM						__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__type_visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT	__attribute__ ((__visibility__("default")))
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT	__attribute__ ((__visibility__("default")))
	#else
		#define LUL_VIS_ENUM

		#define LUL_VIS_CLASS_TEMPLATE				__attribute__ ((__visibility__("default")))

		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT
		#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT
	#endif

	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_EXPORT
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_IMPORT

	#define LUL_VIS_MEMBER_CLASS_TEMPLATE			__attribute__ ((__visibility__("hidden")))

	#define LUL_VIS_MEMBER_FUNCTION_TEMPLATE		__attribute__ ((__visibility__("hidden")))
		//	FIXME There is a clang defect which requires an extraneous “inline”
		//	declaration to be prepended here; it’s currently unknown if gcc is
		//	impacted by the same defect.

	#define LUL_VIS_FUNC_EXPORT						__attribute__ ((__visibility__("default")))
	#define LUL_VIS_FUNC_IMPORT						__attribute__ ((__visibility__("default")))

	//	SEEME The first definition is a fallback if it turns out I’ve
	//	misunderstood the requirements on general inline functions.
//	#define LUL_VIS_INLINE_FUNC						LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC
	#define LUL_VIS_INLINE_FUNC						__attribute__ ((__visibility__("hidden")))

	//	SEEME Using the internal_linkage attribute is equivalent to using the
	//	static keyword in C, and is semantically preferrable to inlining.
	//	However, what we -really- want is be able to strip this down to just
	//	LUL_VIS_HIDDEN, but this requires linker support we don’t have, yet.
	//	See <http://lists.llvm.org/pipermail/cfe-dev/2018-July/058450.html> and
	//	<https://reviews.llvm.org/D49240?id=155185> for details.
	#if __has_attribute (internal_linkage)
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), internal_linkage))
	#else
		#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC	__attribute__ ((__visibility__("hidden"), __always_inline__))
	#endif

	#define LUL_VIS_OVERLOADABLE_FUNC_EXPORT		__attribute__ ((__visibility__("default")))
	#define LUL_VIS_OVERLOADABLE_FUNC_IMPORT		__attribute__ ((__visibility__("default")))

	#define LUL_VIS_EXTERN_EXPORT
	#define LUL_VIS_EXTERN_IMPORT

	#define LUL_VIS_EXCEPTION_EXPORT				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_IMPORT				__attribute__ ((__visibility__("default")))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


	//	Set up identifiers
	#define LUL_NAME_COMPILER u8"gcc version " 		__VERSION__
	#define LUL_TARGET_COMPILER_GCC					1

#elif defined (_MSC_VER) && defined (_WIN32)
	//	Visual C++ targeting Windows; _WIN32 is also defined for 64-bit
	//	SEEME This must always appear last, as other compilers have a tendency
	//	to emulate MSVC by defining its macros.
	#if (_MSC_VER < 1914)
		#error "Settings are only known to be valid for MSVS 2017 15.7+."
	#endif

	#if __cplusplus < 201703L
		#error "std=c++17 or higher is required"
	#endif


	//	Identify processor.
	//	Note that AMD64 and x86_64 have the same identifier.
	#if defined (_M_IX86)
		#define LUL_TARGET_CPU_X86					1
		#define LUL_TARGET_CPU_X86_FAMILY			1
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#elif defined (_M_X64)
		#define LUL_TARGET_CPU_X86_64				1
		#define LUL_TARGET_CPU_X86_FAMILY			1
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#elif defined (_M_IA64)
		#define LUL_TARGET_CPU_IA64					1
		#define LUL_TARGET_CPU_X86_FAMILY			1
		#define LUL_TARGET_RT_LITTLE_ENDIAN			1
	#else
		#error "Processor unknown. Update lulCompilerSetup.hpp."
	#endif


	//	Identify vector instruction sets, if any.
	#ifdef _M_IX86_FP
		#if (_M_IX86_FP == 1)
			//	32-bit processor
			#define LUL_TARGET_VEC_SSE				1
		#elif (_M_IX86_FP == 2)
			//	32-bit processor
			#define LUL_TARGET_VEC_SSE				1
			#define LUL_TARGET_VEC_SSE2				1
		#else
			#pragma message ("Unknown value given by _M_IX86_FP: " #_M_IX86_FP)
		#endif
	#else
		#if (defined(_M_AMD64) || defined(_M_X64))
			//	64-bit processor
			#define LUL_TARGET_VEC_SSE				1
			#define LUL_TARGET_VEC_SSE2				1

			//	SEEME There are no independent tests for these in MSVC, and
			//	it’s unclear if support for these particular instruction sets
			//	is implied for 64-bit architectures, as is the case for SSE1
			//	and SSE2; we make an assumption, as these should be available
			//	in any post-2011 Intel-class CPU.
			#define LUL_TARGET_VEC_SSE3				1
			#define LUL_TARGET_VEC_SSE41			1
			#define LUL_TARGET_VEC_SSE42			1
		#else
			#error "Unsupported processor. Update lulCompilerSetup.hpp."
		#endif

		#ifdef __AVX__
			#define LUL_TARGET_VEC_AVX				1
		#endif

		#ifdef __AVX2__
			#define LUL_TARGET_VEC_AVX2				1
		#endif

		//	SEEME There appear to be no intrinsics for detecting AVX-512
		//	support in MSVC, yet, though support may be undocumented or
		//	under-reported. It‘s currently moot, though, as we don’t support
		//	this instrcution set family at this time.
	#endif


	//	SEEME These are unverified for Win32
	#define LUL_TYPE_DOUBLE_GT_FLOAT				1
	#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE			1


	//	We ignore the possibility of other OS’s
	#define LUL_TARGET_RT_COFF						1


	#define LUL_TYPE_HAS_INT64						1
		//	SEEME We assume the availability of 64-bit ints under supported
		//	versions of MSVC, and also assume the unavaibility of intrinsic
		//	128-bit ints.

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1

	#define LUL_TYPE_WCHAR_T_IS_16_BITS				1


	//	Continuing the Windows tradition of security through annoyance...
	//	Of course, it gets better: in order for this to work, it actually
	//	has to be defined before any includes, even the ones that should
	//	pose no problem. In practice, this definition is probably not coming
	//	early enough, so there will still need to be another one, perhaps in a
	//	project-level property sheet. Consider this documentation.
	#define _CRT_SECURE_NO_WARNINGS					1

	//	Standardize on a gcc-ism.
	//	SEEME This violates the rule about defining preprocessor macros
	//	with double-underscores (among other things). The usage should be
	//	safe.
	#ifndef __PRETTY_FUNCTION__
		#define __PRETTY_FUNCTION__					__FUNCSIG__
	#endif


	//	Test for C++98 features.
	//	These can be conditionally disabled.
	//	SEEME _CPPUNWIND is not an exact analog, but seems to serve.
	#if _CPPUNWIND
		#define LUL_CPP98_EXCEPTIONS							199711L
	#endif

	#if _CPPRTTI
		#define LUL_CPP98_RTTI									199711L
	#endif

	//	Test for C++11 features.
	//	These can be conditionally disabled.
	//	SEEME There doesn’t seem to be a way to determine whether this has
	//	been disabled (which can be done with “/Zc:threadSafeInit-”). We just
	//	assume it hasn’t been disabled, which is not ideal.
	#define LUL_CPP11_THREADSAFE_STATIC_INIT					200806L

	//	C++14 features
	//	FIXME C++2a These feature detection tests will become more manageable
	//	once Microsoft starts integrating the now-mandated SD-6 macros, but
	//	we’ll also have to fix some of this hard-coding.
	//	VS 2017 15.0
	#if (_MSC_VER >= 1900)
		//	SEEME There doesn’t seem to be a way to determine whether this has
		//	been disabled (which can be done with “/Zc:sizedDealloc-”). We just
		//	assume it hasn’t been disabled, which is not ideal.
		#define LUL_CPP14_SIZED_DEALLOCATION					201309L
	#endif

	//	C++17 features
	//	VS 2017 15.5
	#if (_MSC_VER >= 1912)
		//	SEEME This is implemented, but it’s unclear if the DR that affected
		//	the proposal has been addressed.
		#define LUL_CPP17_TEMPLATE_TEMPLATE_ARGS				201611L
	#endif

	//	C++2a features
	//	VS 2017 15.7
	#if (_MSC_VER >= 1914)
		//	SEEME As of this time, MSVC has implemented none of the C++2a
		//	language features, though 1915 is laying the groundwork for a
		//	number of them.
	#endif

	//	C99 features
	#if (_MSC_VER >= 1914)
		//	SEEME Support for a conforming C preprocessor is in the works;
		//	as of VS 2017 5.8 a previewable C99-conformant preprocessor is
		//	available for testing; a fully C++2a-compliant one will be made
		//	available to /permissive- mode soon. The current support may be
		//	sufficient for our needs, but testing needs to be done.
//		#define LUL_C99_PREPROCESSOR							1
	#endif


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN

	#define LUL_VIS_ENUM

	#define LUL_VIS_CLASS_TEMPLATE

	#define LUL_VIS_CLASS_EXPORT					__declspec(dllexport)
	#define LUL_VIS_CLASS_IMPORT					__declspec(dllimport)

	#define LUL_VIS_EXTERN_CLASS_TEMPLATE_EXPORT
	#define LUL_VIS_EXTERN_CLASS_TEMPLATE_IMPORT	__declspec(dllimport)

	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_EXPORT	__declspec(dllexport)
	#define LUL_VIS_CLASS_TEMPLATE_INSTANTIATION_IMPORT

	#define LUL_VIS_MEMBER_CLASS_TEMPLATE

	#define LUL_VIS_MEMBER_FUNCTION_TEMPLATE

	#define LUL_VIS_FUNC_EXPORT						__declspec(dllexport)
	#define LUL_VIS_FUNC_IMPORT						__declspec(dllimport)

	//	SEEME These appear to be required by DLLs, as they like to save
	//	fallback versions of inline functions as object code in case the
	//	header version is unavailable for some reason; if not for this
	//	requirement, these would just be LUL_VIS_HIDDEN.
	#define LUL_VIS_INLINE_FUNC						__forceinline
	#define LUL_VIS_INLINE_TEMPLATE_MEMBER_FUNC		__forceinline

	#define LUL_VIS_OVERLOADABLE_FUNC_EXPORT		__declspec(dllexport)
	#define LUL_VIS_OVERLOADABLE_FUNC_IMPORT

	#define LUL_VIS_EXTERN_EXPORT					__declspec(dllexport)
	#define LUL_VIS_EXTERN_IMPORT					__declspec(dllimport)

	#define LUL_VIS_EXCEPTION_EXPORT				__declspec(dllexport)
	#define LUL_VIS_EXCEPTION_IMPORT				__declspec(dllimport)


	//	Class decorators
	#define LUL_CLASS_FORCE_EBCO					__declspec(empty_bases)


	//	Function calling conventions
	#define LUL_FUNC_CALL_C(LUL_func_name_)			__cdecl LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		__stdcall LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(__cdecl *LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(__stdcall *LUL_func_name_)


	//	Set up identifiers
	#define LUL_NAME_COMPILER u8"MSVC version " 	LUL_Stringify_ (_MSC_VER)
	#define LUL_TARGET_COMPILER_MSVC				1

#else
	//	Unsupported compiler; we don’t bother guessing.
	#error "Unsupported compiler"
#endif


/*------------------------------------------------------------------------------
	These -must- be defined by this point.
*/

#if !defined (LUL_NAME_COMPILER)
	#error "LUL_NAME_COMPILER must be defined"
#endif	//	LUL_NAME_COMPILER


/*------------------------------------------------------------------------------
	Set up the rest of the target compiler conditionals
*/

#ifndef LUL_TARGET_COMPILER_CLANG
	#define LUL_TARGET_COMPILER_CLANG			0
#endif

#ifndef LUL_TARGET_COMPILER_GCC
	#define LUL_TARGET_COMPILER_GCC				0
#endif

#ifndef LUL_TARGET_COMPILER_MSVC
	#define LUL_TARGET_COMPILER_MSVC			0
#endif


/*------------------------------------------------------------------------------
	Set up the rest of the CPU conditionals
*/

#ifndef LUL_TARGET_CPU_ARM
	#define LUL_TARGET_CPU_ARM					0
#endif

#ifndef LUL_TARGET_CPU_ARM_64
	#define LUL_TARGET_CPU_ARM_64				0
#endif

#ifndef LUL_TARGET_CPU_X86
	#define LUL_TARGET_CPU_X86					0
#endif

#ifndef LUL_TARGET_CPU_X86_64
	#define LUL_TARGET_CPU_X86_64				0
#endif

#ifndef LUL_TARGET_CPU_IA64
	#define LUL_TARGET_CPU_IA64					0
#endif


#ifndef LUL_TARGET_CPU_ARM_FAMILY
	#define LUL_TARGET_CPU_ARM_FAMILY			0
#endif

#ifndef LUL_TARGET_CPU_X86_FAMILY
	#define LUL_TARGET_CPU_X86_FAMILY			0
#endif


#if LUL_TARGET_CPU_ARM_64
	#define LUL_NAME_TARGET_CPU					u8"ARM64"
#elif LUL_TARGET_CPU_ARM
	#define LUL_NAME_TARGET_CPU					u8"ARM"
#elif LUL_TARGET_CPU_IA64
	#define LUL_NAME_TARGET_CPU					u8"ia64"
#elif LUL_TARGET_CPU_X86_64
	#define LUL_NAME_TARGET_CPU					u8"x86_64"
#elif LUL_TARGET_CPU_X86
	#define LUL_NAME_TARGET_CPU					u8"i386"
#else
	#error "LUL_NAME_TARGET_CPU must be defined"
#endif


/*------------------------------------------------------------------------------
	Set up the rest of the vector conditionals
*/

#ifndef LUL_TARGET_VEC_SSE
	#define LUL_TARGET_VEC_SSE					0
#endif

#ifndef LUL_TARGET_VEC_SSE2
	#define LUL_TARGET_VEC_SSE2					0
#endif

#ifndef LUL_TARGET_VEC_SSE3
	#define LUL_TARGET_VEC_SSE3					0
#endif

#ifndef LUL_TARGET_VEC_SSE41
	#define LUL_TARGET_VEC_SSE41				0
#endif

#ifndef LUL_TARGET_VEC_SSE42
	#define LUL_TARGET_VEC_SSE42				0
#endif

#ifndef LUL_TARGET_VEC_AVX
	#define LUL_TARGET_VEC_AVX					0
#endif

#ifndef LUL_TARGET_VEC_AVX2
	#define LUL_TARGET_VEC_AVX2					0
#endif


/*------------------------------------------------------------------------------
	Set up the rest of the RT conditionals
*/

#ifndef LUL_TARGET_RT_LITTLE_ENDIAN
	#define LUL_TARGET_RT_LITTLE_ENDIAN			0
#endif

#ifndef LUL_TARGET_RT_BIG_ENDIAN
	#define LUL_TARGET_RT_BIG_ENDIAN			0
#endif

#ifndef LUL_TARGET_RT_32_BIT
	#define LUL_TARGET_RT_32_BIT				0
#endif

#ifndef LUL_TARGET_RT_64_BIT
	#define LUL_TARGET_RT_64_BIT				0
#endif

#ifndef LUL_TARGET_RT_MACHO
	#define LUL_TARGET_RT_MACHO					0
#endif


/*------------------------------------------------------------------------------
	Set up TYPE values if they weren’t previously defined.
*/

#ifndef LUL_TYPE_LONG_DOUBLE_GT_DOUBLE
	#define LUL_TYPE_LONG_DOUBLE_GT_DOUBLE		0
#endif

#ifndef LUL_TYPE_DOUBLE_GT_FLOAT
	#define LUL_TYPE_DOUBLE_GT_FLOAT			0
#endif

#ifndef LUL_TYPE_HAS_INT64
	#define LUL_TYPE_HAS_INT64					0
#endif

#ifndef LUL_TYPE_HAS_INT128
	#define LUL_TYPE_HAS_INT128					0
#endif

#ifndef LUL_TYPE_EXACT_WIDTH_INTEGERS
	#define LUL_TYPE_EXACT_WIDTH_INTEGERS		0
#endif

#ifndef LUL_TYPE_WCHAR_T_IS_16_BITS
	#define LUL_TYPE_WCHAR_T_IS_16_BITS			0
#endif


/*------------------------------------------------------------------------------
	Set up Feature conditionals if they weren’t previously defined.
*/

#ifndef LUL_FEATURE_CONSTEXPR_INTRINSICS
	#define LUL_FEATURE_CONSTEXPR_INTRINSICS	0
#endif

/*------------------------------------------------------------------------------
	Set up C++98 values if they weren’t previously defined.
*/

#ifndef LUL_CPP98_EXCEPTIONS
	#define LUL_CPP98_EXCEPTIONS											0
#endif

#ifndef LUL_CPP98_RTTI
	#define LUL_CPP98_RTTI													0
#endif


/*------------------------------------------------------------------------------
	Set up C++11 values if they weren’t previously defined.
*/

#ifndef LUL_CPP11_MINIMAL_GARBAGE_COLLECTION
	#define LUL_CPP11_MINIMAL_GARBAGE_COLLECTION							0
#endif

#ifndef LUL_CPP11_THREADSAFE_STATIC_INIT
	#define LUL_CPP11_THREADSAFE_STATIC_INIT								0
#endif


/*------------------------------------------------------------------------------
	Set up C++14 values if they weren’t previously defined.
*/

#ifndef LUL_CPP14_SIZED_DEALLOCATION
	#define LUL_CPP14_SIZED_DEALLOCATION									0
#endif


/*------------------------------------------------------------------------------
	Set up C++17 values if they weren’t previously defined.
*/

#ifndef LUL_CPP17_TEMPLATE_TEMPLATE_ARGS
	#define LUL_CPP17_TEMPLATE_TEMPLATE_ARGS								0
#endif


/*------------------------------------------------------------------------------
	Set up C++2a values if they weren’t previously defined.
*/

#ifndef LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS
	#define LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS						0
#endif

#ifndef LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS
	#define LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS							0
#endif

#ifndef LUL_CPP2A_ATTRIBUTE_LIKELY
	#define LUL_CPP2A_ATTRIBUTE_LIKELY										0
#endif

#ifndef LUL_CPP2A_ATTRIBUTE_UNLIKELY
	#define LUL_CPP2A_ATTRIBUTE_UNLIKELY									0
#endif

#ifndef LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS
	#define LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS			0
#endif

#ifndef LUL_CPP2A_CONCEPTS
	#define LUL_CPP2A_CONCEPTS												0
#endif

#ifndef LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS
	#define LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS				0
#endif

#ifndef LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION
	#define LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION							0
#endif

#ifndef LUL_CPP2A_CONTRACTS
	#define LUL_CPP2A_CONTRACTS												0
#endif

#ifndef LUL_CPP2A_COROUTINES
	#define LUL_CPP2A_COROUTINES											0
#endif

#ifndef LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS
	#define LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS					0
#endif

#ifndef LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS
	#define LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS			0
#endif

#ifndef LUL_CPP2A_DESIGNATED_INITIALIZERS
	#define LUL_CPP2A_DESIGNATED_INITIALIZERS								0
#endif

#ifndef LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES
	#define LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES		0
#endif

#ifndef LUL_CPP2A_EXPLICIT_BOOL
	#define LUL_CPP2A_EXPLICIT_BOOL											0
#endif

#ifndef LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR
	#define LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR					0
#endif

#ifndef LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION
	#define LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION	0
#endif

#ifndef LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS
	#define LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS					0
#endif

#ifndef LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE
	#define LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE					0
#endif

#ifndef LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR
	#define LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR							0
#endif

#ifndef LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS
	#define LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS			0
#endif

#ifndef LUL_CPP2A_TYPENAME_OPTIONAL
	#define LUL_CPP2A_TYPENAME_OPTIONAL										0
#endif

#ifndef LUL_CPP2A_VA_OPT
	#define LUL_CPP2A_VA_OPT												0
#endif


/*------------------------------------------------------------------------------
	Set up C99 values if they weren’t previously defined.
*/

#ifndef LUL_C99_PREPROCESSOR
	#define LUL_C99_PREPROCESSOR											0
#endif


/*------------------------------------------------------------------------------
	Set up any unitialized class decorators
*/

#ifndef LUL_CLASS_FORCE_EBCO
	#define LUL_CLASS_FORCE_EBCO
#endif


/*------------------------------------------------------------------------------
	Set up any unitialized function calling conventions
*/

#ifndef LUL_FUNC_CALL_C
	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
#endif

#ifndef LUL_FUNC_CALL_STD
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
#endif

#ifndef LUL_FUNC_CALLBACK_C
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		*LUL_func_name_
#endif

#ifndef LUL_FUNC_CALLBACK_STD
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	*LUL_func_name_
#endif


/*------------------------------------------------------------------------------
	Set up any unitialized intrinsic functions
*/

#ifndef LUL_BUILTIN_likely
	#define LUL_BUILTIN_likely(LUL_expr_)			LUL_expr_
#endif

#ifndef LUL_BUILTIN_unlikely
	#define LUL_BUILTIN_unlikely(LUL_expr_)			LUL_expr_
#endif
