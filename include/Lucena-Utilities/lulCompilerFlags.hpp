/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“CompilerFlags.hpp”
	Copyright © 2007-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This header contains feature tests and wrappers to safeguard a compiler
	(and Standard Library) functionality baseline. We currently require full
	C++14 support (or at least emulation thereof) and significant C++17
	support (with emulation allowed). Note that this header will evolve in
	a compatibility-breaking way until the full set of supported compilers
	offer full support for C++17, at which point it will use C++17 as a
	reference baseline and simply track compliance with newer language
	features.

	Note that we set all conditionals unless we explicitly state otherwise in
	the conditional description; those that are not met get set to 0. By
	default, we set conditionals that are met to 1, but they could conceivably
	be set to any non-0 number, and tests need to reflect that.

	Generally, we don’t bother testing for things that are guaranteed by our
	minimum-supported compilers. For example, all supported compilers have C99-
	style ints when cstdint is included, so we don’t test for the presence of
	support. This may change in the future if a compiler we’d like to support
	violates this standard.

	As a general rule, if a feature is available and stable, it is assumed to
	be used. This is not boost, and we do not seek to incent or support
	thousands of micro-variants on the Standard. The sole objective of this
	library is to facilitate interoperability between otherwise-conforming
	modern compilers.

	LUL_COMPILER_IDENTIFIER
	LUL_STANDARD_LIBRARY_IDENTIFIER
	These  are human-readable strings which identify the tools used for
	compilation. They may include version infrmation as part of the string, but
	are not required to do so.

	LUL_TARGET_CPU
	LUL_TARGET_CPU_IDENTIFIER
	These conditionals specify which microprocessor instruction set is being
	generated.	At most one of these is 1, the rest are 0.
	SEEME There used to be more of these, but they’ve been largely mooted.
	Candidates for additions include console-specific processors/families, and
	explicit differentiation for AMD processors/families.

		LUL_TARGET_CPU_X86							generic 32-bit x86
		LUL_TARGET_CPU_X86_64						generic 64-bit x86
		LUL_TARGET_CPU_IA64							generic 64-bit Itanium
		LUL_TARGET_CPU_ARM							generic 32-bit ARM
		LUL_TARGET_CPU_ARM_64						generic 64-bit ARM

		LUL_TARGET_CPU_ARM_FAMILY
		LUL_TARGET_CPU_X86_FAMILY

	LUL_TARGET_CPU_IDENTIFIER is a human-readable string broadly identifying
	the target CPU, as per the above macros.


	LUL_TARGET_VEC
	These conditionals specify which vector instruction set is being generated,
	if any. Multiple conditionals may be true, but some are mutually exclusive.

		LUL_TARGET_VEC_SSE
		LUL_TARGET_VEC_SSE2
		LUL_TARGET_VEC_SSE3
		LUL_TARGET_VEC_SSE41
		LUL_TARGET_VEC_SSE42
		LUL_TARGET_VEC_AVX
		LUL_TARGET_VEC_AVX2

	Note that there is no corresponding test for AVX-512, as it’s not a
	monolithic instruction set, and we don’t have the operational experience
	needed to evaluate a meaningful breakdown beyond just mirroring any
	predfined macros the compiler may happen to have.

		//	LUL_TARGET_VEC_AVX512

	SEEME We don’t currently independently check for SSE-Math and similar
	instruction splits; this may be a defect.
	SEEME There used to be more of these, but they’ve been largely mooted.
	Candidates for additions include console-specific vector instruction sets,
	and explicit differentiation for AMD instruction sets.


	LUL_TARGET_OS
	LUL_TARGET_OS_IDENTIFIER
	These conditionals specify in which Operating System the generated code
	will run. At most one of the these is 1, the rest are 0 (except for
	LUL_TARGET_OS_IOS, which is set whenever LUL_TARGET_OS_IOS_SIM is set, but
	can also be set alone).
	SEEME Candidates for additions include console-specific operating systems,
	Android, tvOS, and watchOS. BSD could conceivably have its own flag, but we
	currently roll it in under LUL_TARGET_OS_X11.
	SEEME These focus primarily on UI characteristics, as opposed to system
	internals, which is why we have LUL_TARGET_OS_X11 and not
	LUL_TARGET_OS_POSIX. A case could be made that we realistically need to
	track both, but in practice - during the implementation of Lucena PAL - no
	practical need was found. It’s possible that once additional platform
	support is added to that library, we’ll find we need to revisit this design
	decision.

		LUL_TARGET_OS_X11
		LUL_TARGET_OS_OSX
		LUL_TARGET_OS_WIN
		LUL_TARGET_OS_IOS
		LUL_TARGET_OS_IOS_SIM

	LUL_TARGET_OS_IDENTIFIER is a human-readable string. It may identify the
	minimum supported version of the target OS, but is not required to do so.


	LUL_TARGET_RT
	These conditionals specify in which runtime the generated code will
	run. This is needed when the OS and CPU support more than one runtime
	(e.g. MacOS X on PPC supports CFM and Mach-O). Note that values are
	descriptive; if the condition is met, the value will be 1, otherwise 0.
	SEEME The executable format conditionals are somewhat arbitrary, but we’ve
	only bothered to define them for cases we’ve encountered in practice.
	Additional operations experience (and supported platforms) will probably
	lead to some changes.

		LUL_TARGET_RT_LITTLE_ENDIAN
		LUL_TARGET_RT_BIG_ENDIAN

		LUL_TARGET_RT_32_BIT
		LUL_TARGET_RT_64_BIT

		LUL_TARGET_RT_COFF
		LUL_TARGET_RT_ELF
		LUL_TARGET_RT_MACHO
		LUL_TARGET_RT_WASM


	LUL_TARGET_API
	These conditionals are used to differentiate between sets of API’s on
	the same processor under the same OS. The first section after _API_ is
	the OS.	The second section is the API set.	Unlike LUL_TARGET_OS and
	LUL_TARGET_CPU, these conditionals are not mutally exclusive. This header
	will attempt to auto-configure all LUL_TARGET_API values, but will often
	need a LUL_TARGET_API value predefined (e.g., in a .prop or .xcconfig file)
	in order to disambiguate.  Note that values are descriptive; if the
	condition is met, the value will be 1, otherwise 0.
	SEEME This is not intended to be an exhaustive list of APIs. Originally,
	it was useful for differentiating between possible supported and available
	Apple API (e.g., QuickDraw, Carbon, Cocoa, and whatever other flavor of the
	week floats in), but it’s academic on platforms that don’t deprecate their
	APIs with abandon. Further operational experience might find us wanting to
	differentiate between other available OS-level APIs.

		LUL_TARGET_API_COCOA

		LUL_TARGET_API_COCOA_TOUCH

		LUL_TARGET_API_WIN32
		LUL_TARGET_API_WIN64


	LUL_PRAGMA
	These conditionals specify whether the compiler supports particular
	#pragma’s. Arguably, these are gratuitous since compilers should ignore
	unrecongnized #pragmas, but besides preventing noisy warnings, these could
	be useful to indicate, for example, the NON-availability of some requisite
	feature, such as tight struct packing.

		LUL_PRAGMA_STRUCT_ALIGN					#pragma options align=mac68k/power/reset
		LUL_PRAGMA_STRUCT_PACKPUSH				#pragma pack(push, n)/pack(pop)


	LUL_TYPE
	These identify characteristics of certain POD types for a given compiler;
	preprocessor definitions are used since sizeof of other operators can’t be
	used by the preprocessor (reliably).

		LUL_TYPE_HAS_INT64
			//	64-bit ints, as int64_t and uint64_t (and possibly long long)

		LUL_TYPE_HAS_INT128
			//	128-bit ints, as __int128_t and __uint128_t

		LUL_TYPE_EXACT_WIDTH_INTEGERS
			//	availability of C99 exact width int types

		LUL_TYPE_DOUBLE_GT_FLOAT
			//	double is distinct from float

		LUL_TYPE_LONG_DOUBLE_GT_DOUBLE
			//	long double is distinct from double

		LUL_TYPE_WCHAR_T_IS_16_BITS
			//	wchar_t is assumed to be a 32-bit integer type if 0 or unset

		LUL_TYPE_HAS_C99_FLOATS
			//	availability of float_t and double_t


	LUL_STDC99
	For compilers that support C99, C99 functions are in the std namespace.
	Compilers that don’t fully support C99 often have C99 functions, but keep
	them in the global namespace. Whenever a C99 function is used, refer to it
	as LUL_STDC99::<func_xxx>, where <func_xxx> is the function name.


	LUL_FEATURE
	Some compilers and platforms do things in a peculiar way that we may need
	to work around or otherwise deal with. Those features are noted here.

		LUL_FEATURE_UTF16_FILE_SYSTEM
			//	It is assumed that the file system uses UTF-8 unless this
			//	feature conditional is set. Really only necessary for Windows.
			//	FIXME We can probably retire this after updating the code to
			//	always use the relevant std::filesystem path types instead of
			//	doing our own conversions.

	LUL_CPPxx
	LUL_Cxx
	Language feature availability flags to indicate whether a given C++ feature
	is supported by the current compiler. Each one is set to a non-zero value
	if available and 0 otherwise; every macro is always set to some value after
	this header has been preprocessed.

	Note that these flags are neither exhaustive nor bounded:

		* Features that are available in all supported compilers do not have
		flags. Currently, features that could conceivably be manually disabled
		in a given compiler (e.g., exception handling) are not tracked, though
		they could be, if necessary.

		* Features that have become universally supported since the inception
		of their feature flag will eventually have their flag removed. This
		library is not intended to be backwards-compatible for all time, only
		to smooth out cross-platform interoperability between current compiler
		versions. This is unlikely to change, but sane user policies regarding
		freezes for libraries and supported compilers for project milestones -
		or even lifetimes - should mitigate this.

		* Features we (the developers) have not had cause to use or care about
		are often not represented right away. Eventually, all language features
		accepted into the Standard are likely to appear here, assuming
		universal support doesn’t come so quickly that a feature flag becomes
		mooted, as per the above criteria.

		* Features from proposals to the Standard may have flags here; in some
		cases, this may occur for proposed features that have not been accepted
		into the standard, even as part of a Technical Specification. This
		typically happens with de facto standards (e.g., variants on symbol
		visibility handling) when it relates to language features. This sort of
		thing is much more common for proposed library features (e.g., <span>
		support was made available long before it was formally voted in).
	
	Note that the macro names mimic those of the equivalent SD-6 macros, where
	available; similarly, the macro values will either be:

		- 0, to indicate no availability, or
		- the SD-6 value equivalent to the available feature support, or
		- 1, if there is no equivalent SD-6 macro

	SEEME Note that this leaves an ambiguous case where a feature may have been
	updated, e.g., as a result of a Defect Report, but a new SD-6 value has not
	been assigned; this can be further complicated if the DR resolution is not
	finalized, meaning that the available feature is in an intermediate state.
	Currently, we have no policy for dealing with this situation, as it has not
	arisen yet in practice. Most likely it would be addressed by assigning an
	arbitrary value (e.g., “last known good value +1”). This comes with its own
	problems, namely that there is no formal policy regarding bumping SD-6
	macro values vs. creating new macros, for example in the case where
	backwards compatibility is affected by a breaking change, which would
	complicate a simple greater-than test against a macro value.

	APIME Previous iterations of this header also attempted to determine
	whether various C99 and C11 features were available. This turned out to be
	impractical and a bit pointless. As a result, such features are only
	tracked - if they are tracked at all - in the context of their
	applicability to a given C++ Standard, e.g., support for the C99
	preprocessor as required by C++11.

	C++98
	These are here only because compilers can be made to disable specific
	features for various reasons and we want to have a universal way of
	determining whether that’s happened.

		LUL_CPP98_EXCEPTIONS
			There -was- an SD-6 macro for this, but it’s been removed for
			standardization. Set to 1 if exceptions are enabled, 0 otherwise.

		LUL_CPP98_RTTI
			There -was- an SD-6 macro for this, but it’s been removed for
			standardization. Set to 1 if RTTI is enabled, 0 otherwise.

	C++11
	Note that all supported compilers support all required features of C++11.

		LUL_CPP11_MINIMAL_GARBAGE_COLLECTION
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2670.htm>
			There is currently no SD-6 macro for this.

		LUL_CPP11_THREADSAFE_STATIC_INIT
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2660.htm>
		__cpp_threadsafe_static_init
			Some compiler can disable this feature if asked; will be set to 0
			if that has happened.

	C++14
	All supported compilers support all features of C++14, but some of them may
	require that certain features be explicitly enabled.

		LUL_CPP14_SIZED_DEALLOCATION
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3778.html>
		__cpp_sized_deallocation
			Some compilers disable this by default since it’s an ABI-breaking
			change; clang, in particular, does this.

	C++17
	All supported compilers support all features of C++17, but some of them may
	require that certain features be explicitly enabled.

		LUL_CPP17_TEMPLATE_TEMPLATE_ARGS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0522r0.html>
		__cpp_template_template_args
			The proposal this is from is intended to resolve a Defect Report,
			but unfortunately introduces a defect of its own. Some compilers
			are disabling this until a revised patch is in.

	C++2a
	These are often difficult or impossible to emulate correctly, so the norm
	is to not bother. However, when it -has- been done, we note it, as well as
	how to make use of the emulated feature.

	SEEME Somewhat awkwardly, a lot of proposed new language and library
	features and fixes don’t include SD-6 macros (yet?); this makes identifying
	availability more challenging than it needs to be. Note that a proposal to
	standardize SD-6 macros was accepted for C++2a, but the current SD-6 itself
	is pretty out-of-date, and it’s unclear whether there will be a fix-up
	prior to FDIS. As a result, most of these are either placeholders awaiting
	testing or depend on compiler version-differentiated activation.

		LUL_CPP2A_ALLOW_LAMBDA_CAPTURE_EQUALS_THIS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0409r2.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_ATTRIBUTE_NO_UNIQUE_ADDRESS
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0840r2.html>
		__has_cpp_attribute(no_unique_address)

		LUL_CPP2A_ATTRIBUTE_LIKELY
		LUL_CPP2A_ATTRIBUTE_UNLIKELY
		LUL_CPP2A_ATTRIBUTES_LIKELY_AND_UNLIKELY
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0479r5.html>
		__has_cpp_attribute(likely)
		__has_cpp_attribute(unlikely)
			We track the availability of each attribute separately, as well as
			provide an aggregate to test for compliance. The aggregate’s value
			will be 0 if either attribute is unavailable, or 1 otherwise.

		LUL_CPP2A_CLASS_TYPES_AS_NON_TYPE_TEMPLATE_PARAMETERS
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0732r2.pdf>
		__cpp_nontype_template_parameter_class

		LUL_CPP2A_CONCEPTS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0734r0.pdf>
		__cpp_concepts

		LUL_CPP2A_CONST_REF_QUALIFIED_POINTERS_TO_MEMBERS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0704r1.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_CONSTEXPR_VIRTUAL_FUNCTION
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1064r0.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_ATTRIBUTE_ASSERT
		LUL_CPP2A_ATTRIBUTE_ENSURES
		LUL_CPP2A_ATTRIBUTE_EXPECTS
		LUL_CPP2A_CONTRACTS
		__has_cpp_attribute(assert)
		__has_cpp_attribute(ensures)
		__has_cpp_attribute(expects)
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0542r5.html>
			This is actually a bundle of attributes using a new syntax; the
			syntax has no SD-6 macro, so we rely on the presence of the
			attributes to determine support. We track the availability of each
			attribute separately, as well as provide an aggregate to test for
			compliance. The aggregate’s value will be 0 if any attribute is
			unavailable, or 1 otherwise.

		LUL_CPP2A_COROUTINES
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0912r1.html>
		__cpp_coroutines
			Note that this is just language support for the <coroutine> header.

		LUL_CPP2A_DEFAULT_CONSTRUCTIBLE_AND_ASSIGNABLE_STATELESS_LAMBDAS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0624r2.pdf>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_DEFAULT_MEMBER_INITIALIZERS_FOR_BIT_FIELDS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0683r1.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_DESIGNATED_INITIALIZERS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0329r4.pdf>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_EFFICIENT_SIZED_DELETE_FOR_VARIABLE_SIZED_CLASSES
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0722r3.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_EXPLICIT_BOOL
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0892r2.html>
		__cpp_explicit_bool

		LUL_CPP2A_INIT_STATEMENTS_FOR_RANGE_BASED_FOR
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0614r1.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_INITIALIZER_LIST_CONSTRUCTORS_IN_CLASS_TEMPLATE_ARGUMENT_DEDUCTION
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0702r1.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_INTEGRATING_OUR_FEATURE_TEST_MACROS
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0941r2.html>
			There is currently no SD-6 macro for this (ironically). It appears
			this is only here to get Microsoft to support SD-6, as the other
			major compilers de facto meet the requirement.

		LUL_CPP2A_PACK_EXPANSION_IN_LAMBDA_INIT_CAPTURE
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0780r2.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_THREE_WAY_COMPARISON_OPERATOR
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0515r0.pdf>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_TEMPLATE_PARAMETER_LIST_FOR_GENERIC_LAMBDAS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0428r2.pdf>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_TYPENAME_OPTIONAL
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0634r2.html>
			There is currently no SD-6 macro for this.

		LUL_CPP2A_VA_OPT
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0306r4.html>
			There is currently no SD-6 macro for this.

	Technical Specifications and Proposals
	Most of these will eventually migrate to one of the above sections, at
	which time the associated macro will be renamed (and possibly aliased).

	LUL_C99
	Feature availability macros to indicate whether a given C99 feature is
	supported by the current compiler. Each one is set to 1 if available and 0
	otherwise.

		LUL_C99_PREPROCESSOR
		SEEME Only MSVS is known to have problems with this, and it appears
		progress is finally being made to fully address it, at which point this
		flag will be retired. Note that if support requires having a particular
		compiler switch set to get compatibility, setting the switch will
		likely be mandated.

	LUL_LIBCPPxx_xxx
	LUL_LIBCPPxx_xxx_EXP
	These relate to library features. They’re set in lulVersionWrapper.hpp,
	but they’re documented here since they may be overridden depending on the
	build environment. Note that features that are unambiguously available
	(e.g., almost the entirety of the C++11 STL) don’t have macros. Generally,
	a Library feature detection macro is only set to 0 here if we know that a
	feature is broken or unavailable, and that it would otherwise be falsely
	recognized as available. A macro is only set to a non-zero value if we
	know that a feature -should- be available, but normal feature detection
	would fail to assign the correct SD-6 macro value; note that in this case,
	the macro may be reset to 0 later if header detection fails. See the notes
	above for LUL_CPPxx as well as in lulVersionWrapper.hpp for additional
	information.

	Note that some of these -also- require language support, or have some other
	dependencies. We try to note such cases when the dependencies exist outside
	of the current Standard baseline, e.g., a C++2a library feature that
	depends on C++2a language support, when the baseline is C++17.

	There is some ambiguity in whether we track the experimental version of a
	feature. Generally, we don’t bother if:

		- no major compiler ever shipped an experimental version
		- the experimental version is/was completely broken
		- we’re transitioning to a new baseline and looking to eliminate cruft

	SEEME Note that regardless of how these macros are set, a feature may be
	available, anyway, if there is a reference implementation for it; see the
	lulConfig.hpp header for details of how this works, and see the relevant
	Wrapper header, if there is one.

	C++17
		LUL_LIBCPP17_ANY
		__cpp_lib_any
			Only needed because of older Apple platforms.  Note that we neither
			track nor use experimental versions of this.

		LUL_LIBCPP17_ELEMENTARY_STRING_CONVERSIONS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0067r5.html>
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0682r1.html>
		__cpp_lib_to_chars
			No wrapper is provided, though a <charconv> wrapper would be a
			reasonable thing to add. Note that this got weird since the
			functions got moved to their own header and their signatures were
			altered, post-Standardization, all as part of a Defect Report
			resolution.

		LUL_LIBCPP17_EXECUTION
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0024r2.html>
		__cpp_lib_execution
			No wrapper is provided; the functionality would be good to have,
			but challenging to implement effectively. Note that by itself, this
			macro does not measure compliance with the referenced Working Group
			paper; use LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS. Note
			that we neither track nor use experimental versions of this.

		LUL_LIBCPP17_FILESYSTEM
		LUL_LIBCPP17_FILESYSTEM_EXP
		__cpp_lib_filesystem
		__cpp_lib_experimental_filesystem
			Only missing on Apple platforms. Unfortunately, we fall back to
			boost for a reference implementation, and the boost version differs
			substantially from the Standard. Also, note that the experimental
			version differs in many ways from the final version, but it’s all
			that is offered until gcc 8 and clang 7.

		LUL_LIBCPP17_HARDWARE_INTERFERENCE_SIZE
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0154r1.html>
		__cpp_lib_thread_hardware_interference_size
			This is set in this header; no wrapper is provided, though the
			functionality would be a reasonable thing to add. Note that we
			neither track nor use experimental versions of this.

		LUL_LIBCPP17_LAUNDER
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0137r1.html>
		__cpp_lib_launder
			AKA, “Replacement of class objects containing reference members.”
			No wrapper is provided, though the functionality would be a
			reasonable thing to add. Note that we neither track nor use
			experimental versions of this.

		LUL_LIBCPP17_OPTIONAL
		__cpp_lib_optional
			Only needed because of older Apple platforms.  Note that we neither
			track nor use experimental versions of this.

		LUL_LIBCPP17_PARALLEL_ALGORITHM
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0024r2.html>
		__cpp_lib_parallel_algorithm
			This is set in this header. No wrapper is provided; the
			functionality would be good to have, but challenging and messy to
			implement effectively. Note that by itself, this macro does not
			measure compliance with the Working Group paper; for that, use
			LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS. Note that we
			neither track nor use experimental versions of this.

		LUL_LIBCPP17_SPLICING_MAPS_AND_SETS
		<http://wg21.link/p0083r3>
		__cpp_lib_node_extract
			This is set in this header; no wrapper is provided, as we’re
			just testing for the presense of member functions in pre-existing
			standard headers. Note that we neither track nor use experimental
			versions of this.

		LUL_LIBCPP17_STANDARDIZATION_OF_PARALLELISM_TS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0024r2.html>
			Note that this macro tracks compliance with the Standard as it
			relates to implementing the referenced Working Group paper; there
			is no correspoding SD-6 macro, or rather, there are -two- relevant
			macros which each track the different features required for
			compliance. We actually track both of those, as well as this,
			separately. As such, this macro is an aggregate: if both
			LUL_LIBCPP17_EXECUTION and LUL_LIBCPP17_PARALLEL_ALGORITHM indicate
			compliance, this gets set to 1; otherwise, it’s set to 0. Note that
			we neither track nor use experimental versions of this, partly
			because the Draft versions tracked a different set of features, in
			different places, which were refactored/replaced.

		LUL_LIBCPP17_VARIANT
		__cpp_lib_variant
			Only needed because of older Apple platforms. Note that we neither
			track nor use experimental versions of this.

	C++2a
	These are all taken from features that have been incorporated into the
	draft Standard currently called C++2a and likely to become C++20.

		LUL_LIBCPP2A_BIT_CAST
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0476r2.html>
		__cpp_lib_bit_cast
			A reference implementation is available, and is mostly compatible
			(lack of constepxr memcpy or equivalent makes full compliance
			challenging).

		LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0355r7.html>
			This is set in this header; no wrapper is provided, though the
			functionality would be nice to add if we didn’t have to write the
			reference implementation ourselves.

		LUL_LIBCPP2A_CONCEPT_LIBRARY
		LUL_LIBCPP2A_CONCEPT_LIBRARY_EXP
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0898r3.pdf>
		__cpp_lib_concepts
		__cpp_lib_experimental_concepts
			No wrapper is provided, though the functionality would be a
			reasonable thing to add.

		LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0202r3.html>
			This is set in this header; no wrapper is provided, as we would
			have to reimplement both headers and work around the potential
			lack of cstring-equivalent compiler intrinsics.

		LUL_LIBCPP2A_COROUTINES
		LUL_LIBCPP2A_COROUTINES_EXP
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4736.pdf>
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4760.pdf>
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0912r2.html>
		__cpp_coroutines (SEEME proxy)
			This does not currently have its own SD-6 macro. No wrapper is
			provided. Note that <coroutine> requires language support.
			SEEME Confusingly, n4736 was voted into the Standard, but n4760 is
			the fixed version of it.

		LUL_LIBCPP2A_EXTENDING_MAKE_SHARED_TO_SUPPORT_ARRAYS
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0674r1.html>
			This is set in this header; no wrapper is provided, though the
			functionality would be a reasonable thing to add.

		LUL_LIBCPP2A_FLOATING_POINT_ATOMIC
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0020r6.html>
			This is set in this header; no wrapper is provided, though the
			functionality would be a reasonable thing to add.

		LUL_LIBCPP2A_LIST_REMOVE_RETURN_TYPE
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0646r1.pdf>
		__cpp_lib_list_remove_return_type
		__cpp_lib_experimental_list_remove_return_type
			No wrapper is provided, as we would have to fully reimplement
			<list> and <forward_list> just to tweak a few member functions.

		LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0415r1.html>
		__cpp_lib_constexpr_complex
		__cpp_lib_experimental_constexpr_complex
			No wrapper is provided, as we would have to reimplement <complex>.

		LUL_LIBCPP2A_SPAN
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0122r7.pdf>
			There is no SD-6 macro, yet. A reference implementation is
			provided and accessible through lulSpanWrapper.hpp.

		LUL_LIBCPP2A_STD_ATOMIC_REF
		<http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p0019r8.html>
		__cpp_lib_atomic_ref
			No wrapper is provided, as a correct reference implementation will
			require significant expertise in the problem domain.

		LUL_LIBCPP2A_STD_ENDIAN
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0463r1.html>
			This is set in this header; no wrapper is provided, though the
			functionality would be a reasonable thing to add; however a general
			implementation may be impossible without compiler/language support
			(because middle-endian, amirite?).

		LUL_LIBCPP2A_STD_REMOVE_CVREF
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0550r2.pdf>
			This is set in this header; a reference implementation is provided
			and accessible through lulTypeTraitsWrapper.hpp.

		LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0457r2.html>
			This is set in this header. No wrapper is provided, as we would
			have to reimplement two headers; these should have been free
			functions anyway, rationale in the proposal notwithstanding.

		LUL_LIBCPP2A_SYNCHRONIZED_BUFFERED_OSTREAM
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0053r7.pdf>
		__cpp_lib_syncstream
			No wrapper is provided.

		LUL_LIBCPP2A_THREE_WAY_COMPARISON_OPERATOR_SUPPORT_COMPARE
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0515r3.pdf>
			This is set in this header; no wrapper will be provided, as
			<compare> is interdependent with the operator <=> language feature.

		LUL_LIBCPP2A_TYPE_IDENTITY
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0887r1.pdf>
			This is set in this header; no wrapper is provided, though an
			implementation would be trivial (cf., std::remove_cvref).

		LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0653r2.html>
			This is set in this header; no wrapper is provided, though the
			functionality would be a reasonable thing to add.

		LUL_LIBCPP2A_VERSION
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0754r2.pdf>
			This is effectively provided by lulVersionWrapper.hpp, which works
			in conjunction with this header to identify all of the available
			Standard Library functionality. Note that unlike <version>, the
			wrapper actually tracks “correct” implementations only (so, for
			example, Xcode’s false positives for <any> and friends won’t be
			reported as support for the library feature); this behavior will be
			carried forward, and so, unlike other wrappers, it is recommended
			to -always- include lulVersionWrapper.hpp, even when including
			<version> directly, and rely on the LUL_LIBCPP_xxx macros where
			they are available. See lulVersionWrapper.hpp for details.

	C++ Technical Specifications
	These are all taken from Technical Specifications that have not (yet?) been
	Standardized. Generally, they are either very far along and likely to clear
	the bar, or very useful and simple enough to implement trivially.
	Generally, these will eventually be aliased (and then moved) into
	LUL_LIBCPP2A_xxx, LUL_LIBCPP20_xxx, LUL_LIBCPP23_xxx, and the like.

	SEEME The TSs can be highly mutable, and support should be considered very
	experimental. No effort at all will be expended to maintain backwards-
	compatibility with a TS once a feature has been voted in.

		LUL_LIBCPPTS_NETWORKING
		LUL_LIBCPPTS_NETWORKING_EXP
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4734.pdf>
		__cpp_lib_net
		__cpp_lib_experimental_net
			Unfortunately, we use boost for a reference implementation, so
			compatibility with the Standard is not guaranteed. Note that the
			wrapper for this feature is lulNetworkingWrapper.hpp.

		LUL_LIBCPPTS_NETWORKING_EXTENSIBLE
		LUL_LIBCPPTS_NETWORKING_EXTENSIBLE_EXP
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4734.pdf>
		__cpp_lib_net_extensible
		__cpp_lib_experimental_net_extensible
			Unfortunately, we use boost for a reference implementation, so
			compatibility with the Standard is not guaranteed. Note that the
			wrapper for this feature is lulNetworkingWrapper.hpp.

		LUL_LIBCPPTS_OBSERVER_PTR
		LUL_LIBCPPTS_OBSERVER_PTR_EXP
		<http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2014/n4282.pdf>
		<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4758.html>
		__cpp_lib_observer_ptr
		__cpp_lib_experimental_observer_ptr
			A reference implementation is available.


	LUL_VIS_xxx
	These describe different levels of symbol visibility. By default, all
	symbols are hidden. LUL_VIS_[ALWAYS_]INLINE functions are always hidden in
	the symbol table, regardless of whether or not the affected function is
	actually inlined.
	SEEME Conceivably, these may need to be subdivided further, as we might be
	lacking sufficient granularity. One factor that could necessitate
	subdivision is module support, and the form it takes, which could require
	differentation between types and templates in their visibility handling.

		LUL_VIS_HIDDEN
			//	Do not export the variable/function/template/type. This is the
			//	default, and does not generally need to be explicitly applied.

		LUL_VIS_EXTERN
			//	Declare a constant/variable/function that is defined elsewhere.
			//	This is specifically useful for headers that refer to library
			//	binaries, as opposed to the same headers when they are used to
			//	build the libraries; macros can be used to flip the meaning
			//	depending on the usage case (see below).

		LUL_VIS_DEFINE
			//	Define a constant/variable/function.

		LUL_VIS_ONLY
			//	Decribe a constant/function that is completely contained in the
			//	header and does not require a symbol to use.

		LUL_VIS_TYPE_EXTERN
			//	Declare a type/template instantiation that is defined
			//	elsewhere. In the template case, this is for explicit extern
			//	template declarations, as opposed to explicit instantiations.

		LUL_VIS_TYPE_DEFINE
			//	Define a type/template. In the template case, this is intended
			//	for use with an explicit instantiation, which is useful when
			//	other declarations of the template are extern in order to avoid
			//	generating multiple copies of the same template instantiation
			//	only to have most of them discarded during linking.

		LUL_VIS_TYPE_ONLY
			//	Decribe a type/template that is completely contained in the
			//	header and does not require a symbol to use. Templates
			//	declarations should always have this decoration.

		LUL_VIS_EXCEPTION_EXTERN
		LUL_VIS_EXCEPTION_DEFINE
			//	Usage is the same as for LUL_VIS_TYPE, except there is no “ONLY”
			//	variant.
			//	APIME We have this because clang does, though it’s not clear
			//	why we’d have to differentiate exception classes from other
			//	classes. Assume Howard knows what he’s doing.

		LUL_VIS_INLINE
		LUL_VIS_ALWAYS_INLINE
			//	Declare and define an inline function.

	Note that LUL_VIS_EXTERN and LUL_VIS_DEFINE must be used in conjunction with
	each other, e.g., in a header file included at build time, LUL_VIS_DEFINE
	could establish public visibility for functions in a library which are then
	accessed through a header with the same functions declared using
	LUL_VIS_EXTERN. For convenience, this may even be the same header, but with
	a wrapper macro to select LUL_VIS_DEFINE when building the library, and
	LUL_VIS_EXTERN when linking to it.


	LUL_CLASS_xxx
	These macros describe class decorators whose details are implementation-
	specific.

		LUL_CLASS_FORCE_EBCO
			//	Force the use of the Empty Base (Class) Optimization. The
			//	decorator should precede the class name in its declaration.
			//	Note that the decorator is used specifically to indicate that
			//	the bases of a given class are empty, not that a given class is
			//	an Empty Base Class; this means it must be applied by the end
			//	user in a large number of practical use cases. Also note that
			//	the decorator must be applied in the most direct descendant to
			//	have effect. For example:
			//
			//		struct Empty1 { };
			//		struct Empty2 { };
			//		struct Derived1 : Empty1, Empty2 { };
			//		struct LUL_CLASS_FORCE_EBCO Derived2 : Derived 1 { };
			//
			//	In this example, neither Derived1 -nor- Derived2 will
			//	necessarily benefit from the EBCO; we can’t guarantee the
			//	behavior. However, if Derived1 -also- has the
			//	LUL_CLASS_FORCE_EBCO applied, then any compiler that supports
			//	the EBCO will apply it to both Derived1 -and- Derived2.
			//
			//	SEEME This is really only necessary when using multiple
			//	inheritance and targeting MSVS; at least VS2015 Update 3 is
			//	required (note that Update 2, which actually introduced the
			//	feature, had a bug which caused it to violate the Standard).
			//	APIME How aggravating is it that you can’t decorate the empty
			//	base class itself instead of having to force a weird
			//	requirement on derived classes? Sadly, we’re at the mercy of
			//	the built-in compiler decorators, here.


	LUL_FUNC_xxx
	These macros describe function calling conventions whose details are
	implementation-specific.

		LUL_FUNC_CALL_C(LUL_func_name_)
			//	C calling convention; default

		LUL_FUNC_CALL_STD(LUL_func_name_)
			//	Std calling convention

		LUL_FUNC_CALLBACK_C(LUL_func_name_)
			//	C-style function pointer

		LUL_FUNC_CALLBACK_STD(LUL_func_name_)
			//	Std-style function pointer


	LUL_BUILTIN_xxx
	These are wrappers for intrinsic functions that may not be available on
	every platform. Where a given function is not available, it may be
	emulated, or simply replaced with a no-op.

		LUL_BUILTIN_likely(LUL_expr_)
			//	branch prediction hinting that something is most likely true;
			//	LUL_expr_ must resolve to a boolean
			//	FIXME C++20 this is now an attribute

		LUL_BUILTIN_unlikely(LUL_expr_)
			//	branch prediction hinting that something is most likely false;
			//	LUL_expr_ must resolve to a boolean
			//	FIXME C++20 this is now an attribute

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
		//	SEEME This must not be lulVersionWrapper.hpp, as that header
		//	depends on this one.
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
	Snow White test

	Note that none of lulCompilerFlag’s LUL_XXX macros are set to 0 until after
	the compiler tests, so it’s sufficient to simply test them for existence.
	Just to be sure, though, we make sure someone didn’t define one of these
	behind our backs, with the following exceptions:

		- LUL_TARGET_OS_xxx should be set by the build system or in some
			similar fashion, as otherwise there may be no way to differentiate
			between certain platforms.

		- LUL_TARGET_API_xxx can be set by the build system or in some
			similar fashion to avoid ambiguities (e.g., determine whether we’re
			targeting Carbon or Cocoa APIs deuner older MacOS X’s). It’s not
			generally necessary to do this any more.

		- LUL_LIBCPPxx_xxx macros are only set here if they could not otherwise
			be set properly in lulVersionWrapper.hpp; see the notes above and
			in lulVersionWrapper.hpp.
*/

#if defined (LUL_COMPILER_IDENTIFIER) || \
	defined (LUL_STANDARD_LIBRARY_IDENTIFIER)

	#error "Don’t define build environment identifiers outside of this file."
#endif	//	Build enviornement identifier check

#if !defined (LUL_TARGET_OS_X11) && \
	!defined (LUL_TARGET_OS_OSX) && \
	!defined (LUL_TARGET_OS_WIN) && \
	!defined (LUL_TARGET_OS_IOS) && \
	!defined (LUL_TARGET_OS_IOS_SIM)

	#error "A LUL_TARGET_OS_xxx must be defined outside of this file."
#endif	//	LUL_TARGET_OS check

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

#if defined (LUL_PRAGMA_STRUCT_ALIGN) || \
	defined (LUL_PRAGMA_STRUCT_PACKPUSH)

	#error "Don’t define LUL_PRAGMA_xxx outside of this file."
#endif	//	LUL_PRAGMA check

#if defined (LUL_TYPE_HAS_INT64) || \
	defined (LUL_TYPE_HAS_INT128) || \
	defined (LUL_TYPE_EXACT_WIDTH_INTEGERS) || \
	defined (LUL_TYPE_DOUBLE_GT_FLOAT) || \
	defined (LUL_TYPE_LONG_DOUBLE_GT_DOUBLE) || \
	defined (LUL_TYPE_WCHAR_T_IS_16_BITS) || \
	defined (LUL_TYPE_HAS_C99_FLOATS)

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
	Compiler conditionals

	Arguably, it would be more desirable to give each compiler its own
	#include file and include them here conditionally rather than go the
	monolithic route. We trade off potential unnecessary recompilation of
	unaffected projects for simplified maintenance at this time, mostly because
	the nature of the library is such that all the compilers tend to require
	updates at the same time, anyway.
*/

#if defined (__clang__) && defined (__llvm__) && defined (__APPLE_CC__)
	//	clang-llvm compiler targeting Apple platforms

	#include <Availability.h>
		//	Special Apple header required for compile-time detection of the
		//	SDK version and the targeted platform.

	//	Note that Apple has repurposed the __clang_major__, __clang_minor__,
	//	and __clang_patchlevel__ macros, so they are no longer in line with
	//	other clang distributions. We can test whether we’re in Apple clang by
	//	looking for __APPLE_CC__, __apple_build_version__, or some other macro.
	//	SEEME Apple LLVM 9.1 is the first release that actually advertises
	//	C++17 support (as per __cplusplus); previously, it only had C++14 with
	//	almost all C++17 extensions. Note that libc++ is still broken due to
	//	runtime issues, as described below, as well as other things.
	#if ((__clang_major__ < 9) || ((__clang_major__ == 9) && (__clang_minor__ < 1)))
		#error "Settings are only valid for Apple LLVM 9.1+"
	#else
		//	Tested with a minimum of Xcode 9.4.0; the bundled compiler and
		//	Standard Library are roughly compatible with clang 5 and libc++,
		//	while adding some Apple-proprietary stuff. Note that Xcode 10.0 is
		//	roughly compatible with clang 6.
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
		#error "Processor unknown. Update lulCompilerFlags."
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


	//	Note that Apple platforms are assumed to get their LUL_TARGET_OS_XXX
	//	settings from their xcconfigs.


	//	Identify executable file fromat
	#ifdef __MACH__
		#define LUL_TARGET_RT_MACHO					1
	#else
		#error "No other executable file formats supported for Apple targets"
	#endif


	#if !__LP64__
		#define LUL_PRAGMA_STRUCT_ALIGN				1
	#endif

	#define LUL_PRAGMA_STRUCT_PACKPUSH				1

	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1


	#if LUL_TARGET_OS_OSX
		//	SEEME This is intended to differentiate between Cocoa and Carbon
		//	API availability, but is not really needed any more.
		#define LUL_TARGET_API_COCOA				LUL_TARGET_RT_MACHO
	#else
		//	We make a not-unreasonable assumption here.
		#define LUL_TARGET_API_COCOA_TOUCH			1
	#endif

	//	SEEME Apparently, __STDC_VERSION__ only gets set if we’re using the C
	//	compiler, so this test doesn’t work. Since we’re requiring C++14 or
	//	greater, we assume the following C99 features are available.
	#if 1	//	__STDC_VERSION__ >= 199901L
		#define LUL_C99_PREPROCESSOR				1

		#define LUL_TYPE_HAS_C99_FLOATS				1
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
	//	the real Standard Lbrary deficiencies...):
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

	//	SEEME Speculative; Xcode 10 seems to be tracking vanilla clang 6.
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


	//	Test for certain Standard Library features; these only include things
	//	that the normal testing done in lulVersionWrapper can’t uncover.

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
		#error "The minimum OS requirement for shared_lock has not been met."
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
	#if (_LIBCPP_VERSION < 6000) ||											\
		((defined (__MAC_OS_X_VERSION_MIN_REQUIRED) &&						\
				(__MAC_OS_X_VERSION_MIN_REQUIRED < 101400)) ||				\
			(defined (__IPHONE_OS_VERSION_MIN_REQUIRED) &&					\
				(__IPHONE_OS_VERSION_MIN_REQUIRED < 120000)) ||				\
			(defined (__TV_OS_VERSION_MIN_REQUIRED) &&						\
				(__TV_OS_VERSION_MIN_REQUIRED < 120000)) ||					\
			(defined (__WATCH_OS_VERSION_MIN_REQUIRED) &&					\
				(__WATCH_OS_VERSION_MIN_REQUIRED < 50000)))
		#define LUL_LIBCPP17_ANY											0L
		#define LUL_LIBCPP17_OPTIONAL										0L
		#define LUL_LIBCPP17_VARIANT										0L
	#endif


	//	SEEME Speculating about future availability in Apple’s libc++ is
	//	fraught, as features fail to be adopted from vanilla clang for any
	//	number of reasons. We assume nothing, and wait.
	#if (_LIBCPP_VERSION >= 6000)
//		#if !__cpp_lib_launder
//			#define LUL_LIBCPP17_LAUNDER									201606L
//		#endif
//
//		#define LUL_LIBCPP2A_STD_REMOVE_CVREF								1L
//		#define LUL_LIBCPP2A_STRING_PREFIX_AND_SUFFIX_CHECKING				1L
//		#define LUL_LIBCPP2A_UTILITY_TO_CONVERT_A_POINTER_TO_A_RAW_POINTER	1L
	#endif

	#if (_LIBCPP_VERSION >= 7000)
//		#define LUL_LIBCPP2A_STD_ENDIAN										1L
	#endif


	//	intrinsic functions
	#define LUL_BUILTIN_likely(LUL_expr_)			__builtin_expect (LUL_expr_, true)
	#define LUL_BUILTIN_unlikely(LUL_expr_)			__builtin_expect (LUL_expr_, false)


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))
	#define LUL_VIS_EXTERN					extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_DEFINE							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_ONLY							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_TYPE_EXTERN				extern	__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_DEFINE						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_ONLY						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_EXCEPTION_EXTERN		extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_DEFINE				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_INLINE							__attribute__ ((__visibility__("hidden"), __always_inline__))
	#define LUL_VIS_ALWAYS_INLINE					__attribute__ ((__visibility__("hidden"), __always_inline__))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


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

	#define LUL_COMPILER_IDENTIFIER u8"clang version " __clang_version__

	#if _LIBCPP_VERSION
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"libc++ version " LUL_Stringify_ (_LIBCPP_VERSION)
	#endif

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
		#error "Processor unknown. Update lulCompilerFlags."
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


	//	SEEME Note that LUL_TARGET_OS_XXX must be set as a pre-defined macro or
	//	by some similar method.


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


	#if !__LP64__
		#define LUL_PRAGMA_STRUCT_ALIGN				1
	#endif

	#define LUL_PRAGMA_STRUCT_PACKPUSH				1

	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1


	#if LUL_TARGET_OS_OSX
		#define LUL_TARGET_API_COCOA				1
	#elif LUL_TARGET_OS_IOS || LUL_TARGET_OS_IOS_SIM
		#define LUL_TARGET_API_COCOA_TOUCH			1
	#endif

	//	SEEME Apparently, __STDC_VERSION__ only gets set if we’re using the C
	//	compiler, so this test doesn’t work. Since we’re requiring C++14 or
	//	greater, we assume the following C99 features are available.
	#if 1	//	__STDC_VERSION__ >= 199901L
		#define LUL_C99_PREPROCESSOR				1

		#define LUL_TYPE_HAS_C99_FLOATS				1
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


	//	Test for certain Standard Library features; these only include things
	//	that the normal testing done in lulVersionWrapper can’t uncover.
	//	SEEME We only test for libc++ features; other Standard Library
	//	implementations can be added as needed.
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
//		#define LUL_LIBCPP2A_CONSTEXPR_FOR_ALGORITHM_AND_UTILITY			1L
//		#define LUL_LIBCPP2A_MORE_CONSTEXPR_FOR_COMPLEX						1L

		//	SEEME This appears to be in, though we await confirmation.
		#define LUL_LIBCPP2A_CALENDAR_AND_TIMEZONE							1L
	#endif

	#if (_LIBCPP_VERSION >= 8000)
		#define LUL_LIBCPP2A_TYPE_IDENTITY									1L
	#endif


	//	intrinsic functions
	#define LUL_BUILTIN_likely(LUL_expr_)			__builtin_expect (LUL_expr_, true)
	#define LUL_BUILTIN_unlikely(LUL_expr_)			__builtin_expect (LUL_expr_, false)


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))
	#define LUL_VIS_EXTERN					extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_DEFINE							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_ONLY							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_TYPE_EXTERN				extern	__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_DEFINE						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_ONLY						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_EXCEPTION_EXTERN		extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_DEFINE				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_INLINE							__attribute__ ((__visibility__("hidden"), __always_inline__))
	#define LUL_VIS_ALWAYS_INLINE					__attribute__ ((__visibility__("hidden"), __always_inline__))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


	//	Set up identifiers
 	#if LUL_TARGET_OS_OSX
		#define LUL_TARGET_OS_IDENTIFIER u8"macOS"
	#elif LUL_TARGET_OS_X11 && defined (__linux__)
		#define LUL_TARGET_OS_IDENTIFIER u8"Linux"
	#elif LUL_TARGET_OS_X11 && defined (__FreeBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"FreeBSD version " LUL_Stringify_ (__FreeBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__NetBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"NetBSD version " LUL_Stringify_ (__NetBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__OpenBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"OpenBSD version " LUL_Stringify_ (__OpenBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__DragonFly__)
		#define LUL_TARGET_OS_IDENTIFIER u8"DragonFly version " LUL_Stringify_ (__DragonFly__)
	#elif LUL_TARGET_OS_WIN
		#define LUL_TARGET_OS_IDENTIFIER u8"Windows"
	#else
		#error "Unknown target platform"
	#endif

	#define LUL_COMPILER_IDENTIFIER u8"clang version " __clang_version__

	#if defined (__GLIBCXX__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCXX__)
	#elif defined (__GLIBCPP__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCPP__)
	#elif _LIBCPP_VERSION
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"libc++ version " LUL_Stringify_ (_LIBCPP_VERSION)
	#else
		//	Probably whatever Dinkumware derivative MSVC uses
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"Unknown Standard Library"
	#endif

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
		#error "Processor unknown. Update lulCompilerFlags."
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


	//	FIXME We ignore the possibility of other targets, e.g., Wayland, and
	//	whatever Android uses, not to mention oddball uses of gcc on Apple
	//	platforms and Windows.
	#define LUL_TARGET_OS_X11						1


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


	#define LUL_PRAGMA_STRUCT_PACKPUSH				1

	#if __SIZEOF_LONG_LONG__ == 8
		#define LUL_TYPE_HAS_INT64					1
	#endif

	#if __SIZEOF_INT128__ == 16
		#define LUL_TYPE_HAS_INT128					1
	#endif

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1

	#define LUL_C99_PREPROCESSOR					1
	#define LUL_TYPE_HAS_C99_FLOATS					1


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


	//	Test for certain Standard Library features; these only include things
	//	that the normal testing done in lulVersionWrapper can’t uncover.

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


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN							__attribute__ ((__visibility__("hidden")))
	#define LUL_VIS_EXTERN					extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_DEFINE							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_ONLY							__attribute__ ((__visibility__("default")))
	#define LUL_VIS_TYPE_EXTERN				extern	__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_DEFINE						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_TYPE_ONLY						__attribute__ ((__type_visibility__("default")))
	#define LUL_VIS_EXCEPTION_EXTERN		extern	__attribute__ ((__visibility__("default")))
	#define LUL_VIS_EXCEPTION_DEFINE				__attribute__ ((__visibility__("default")))
	#define LUL_VIS_INLINE							__attribute__ ((__visibility__("hidden"), __always_inline__))
	#define LUL_VIS_ALWAYS_INLINE					__attribute__ ((__visibility__("hidden"), __always_inline__))

	#define LUL_FUNC_CALL_C(LUL_func_name_)			LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(*LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(*LUL_func_name_)


	//	Set up identifiers
 	#if LUL_TARGET_OS_OSX
		#define LUL_TARGET_OS_IDENTIFIER u8"macOS"
	#elif LUL_TARGET_OS_X11 && defined (__linux__)
		#define LUL_TARGET_OS_IDENTIFIER u8"Linux"
	#elif LUL_TARGET_OS_X11 && defined (__FreeBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"FreeBSD version " LUL_Stringify_ (__FreeBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__NetBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"NetBSD version " LUL_Stringify_ (__NetBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__OpenBSD__)
		#define LUL_TARGET_OS_IDENTIFIER u8"OpenBSD version " LUL_Stringify_ (__OpenBSD__)
	#elif LUL_TARGET_OS_X11 && defined (__DragonFly__)
		#define LUL_TARGET_OS_IDENTIFIER u8"DragonFly version " LUL_Stringify_ (__DragonFly__)
	#elif LUL_TARGET_OS_WIN
		#define LUL_TARGET_OS_IDENTIFIER u8"Windows"
	#else
		#error "Unknown target platform"
	#endif

	#define LUL_COMPILER_IDENTIFIER u8"gcc version " __VERSION__

	#if defined (__GLIBCXX__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCXX__)
	#elif defined (__GLIBCPP__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCPP__)
	#elif _LIBCPP_VERSION
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"libc++ version " LUL_Stringify_ (_LIBCPP_VERSION)
	#else
		//	Probably whatever Dinkumware derivative MSVC uses
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"Unknown Standard Library"
	#endif

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
		#error "Processor unknown. Update lulCompilerFlags."
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
			#error "Unsupported processor. Update lulCompilerFlags."
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


	//	We make no assumptions about the addressing model based on processor,
	//	instead relying on intrinsics.
	#ifdef _WIN64
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


	//	We ignore the possibility of other OS’s
	#define LUL_TARGET_OS_WIN						1
	#define LUL_TARGET_RT_COFF						1


	#define LUL_PRAGMA_STRUCT_PACKPUSH				1

	//	SEEME We assume the availability of 64-bit ints under supported
	//	versions of MSVC, and also assume the unavaibility of intrinsic 128-bit
	//	ints.
	#define LUL_TYPE_HAS_INT64						1

	#define LUL_TYPE_EXACT_WIDTH_INTEGERS			1

	#define LUL_TYPE_WCHAR_T_IS_16_BITS				1


	//	Realistically, this belongs somewhere other than in what is ostensibly
	//	a compiler test, but it’s the best fit for now.
	#define LUL_FEATURE_UTF16_FILE_SYSTEM			1


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

	//	C11 features
	//	SEEME Some C11 features are implemented, some are in the works, and
	//	some will “never” be implemented. Most of the C11 Standard Library is
	//	implemented.


	//	Test for certain Standard Library features; these only include things
	//	that the normal testing done in lulVersionWrapper can’t uncover.

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


	//	Symbol visibility macros
	#define LUL_VIS_HIDDEN
	#define LUL_VIS_EXTERN					extern	__declspec(dllimport)
	#define LUL_VIS_DEFINE							__declspec(dllexport)
	#define LUL_VIS_ONLY
	#define LUL_VIS_TYPE_EXTERN				extern	__declspec(dllimport)
	#define LUL_VIS_TYPE_DEFINE						__declspec(dllexport)
	#define LUL_VIS_TYPE_ONLY
	#define LUL_VIS_EXCEPTION_EXTERN		extern	__declspec(dllimport)
	#define LUL_VIS_EXCEPTION_DEFINE				__declspec(dllexport)
	#define LUL_VIS_INLINE							__forceinline
	#define LUL_VIS_ALWAYS_INLINE					__forceinline


	//	Class decorators
	#define LUL_CLASS_FORCE_EBCO					__declspec(empty_bases)


	//	Function calling conventions
	#define LUL_FUNC_CALL_C(LUL_func_name_)			__cdecl LUL_func_name_
	#define LUL_FUNC_CALL_STD(LUL_func_name_)		__stdcall LUL_func_name_
	#define LUL_FUNC_CALLBACK_C(LUL_func_name_)		(__cdecl *LUL_func_name_)
	#define LUL_FUNC_CALLBACK_STD(LUL_func_name_)	(__stdcall *LUL_func_name_)


	//	WinAPI configuration
	#define WIN32_LEAN_AND_MEAN
		//	FIXME Maybe not super-smart to set this up globally

	#define NOMINMAX


	//	Set up identifiers
 	#if LUL_TARGET_OS_OSX
		#define LUL_TARGET_OS_IDENTIFIER u8"macOS"
	#elif LUL_TARGET_OS_WIN
		#define LUL_TARGET_OS_IDENTIFIER u8"Windows"
	#else
		#error "Unknown target platform"
	#endif

	#define LUL_COMPILER_IDENTIFIER u8"MSVC version " LUL_Stringify_ (_MSC_VER)

	#if defined (__GLIBCXX__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCXX__)
	#elif defined (__GLIBCPP__)
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"GNU libstdc++ version " LUL_Stringify_ (__GLIBCPP__)
	#elif _LIBCPP_VERSION
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"libc++ version " LUL_Stringify_ (_LIBCPP_VERSION)
	#else
		//	Probably whatever Dinkumware derivative MSVC uses
		#define LUL_STANDARD_LIBRARY_IDENTIFIER u8"MSVC Standard Library version " LUL_Stringify_ (_MSC_VER)
	#endif

#else
	//	Unsupported compiler; we don’t bother guessing.
	#error "Unsupported compiler"
#endif


/*------------------------------------------------------------------------------
	These -must- be defined by this point.
*/

#if !defined (LUL_COMPILER_IDENTIFIER) || \
	!defined (LUL_STANDARD_LIBRARY_IDENTIFIER)

	#error "Build system identifiers must be defined"
#endif	//	Build environment check

#if !defined (LUL_TARGET_OS_IDENTIFIER)

	#error "LUL_TARGET_OS_IDENTIFIER must be defined"
#endif	//	LUL_TARGET_OS_IDENTIFIER check


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
	#define LUL_TARGET_CPU_IDENTIFIER					u8"ARM64"
#elif LUL_TARGET_CPU_ARM
	#define LUL_TARGET_CPU_IDENTIFIER					u8"ARM"
#elif LUL_TARGET_CPU_IA64
	#define LUL_TARGET_CPU_IDENTIFIER					u8"ia64"
#elif LUL_TARGET_CPU_X86_64
	#define LUL_TARGET_CPU_IDENTIFIER					u8"x86_64"
#elif LUL_TARGET_CPU_X86
	#define LUL_TARGET_CPU_IDENTIFIER					u8"i386"
#else
	#error "LUL_TARGET_CPU_IDENTIFIER must be defined"
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


/*------------------------------------------------------------------------------
	Set up PRAGMA values if they weren’t previously defined.
*/

#ifndef LUL_PRAGMA_STRUCT_ALIGN
	#define LUL_PRAGMA_STRUCT_ALIGN				0
#endif

#ifndef LUL_PRAGMA_STRUCT_PACK
	#define LUL_PRAGMA_STRUCT_PACK				0
#endif

#ifndef LUL_PRAGMA_STRUCT_PACKPUSH
	#define LUL_PRAGMA_STRUCT_PACKPUSH			0
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

#ifndef LUL_TYPE_HAS_C99_FLOATS
	#define LUL_TYPE_HAS_C99_FLOATS				0
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


/*------------------------------------------------------------------------------
	Determine what namespace C99 functions should be in. Of course, if the
	function isn’t implemented even as an extension, we have a whole other
	problem, but at least it’ll be a compile-time problem.

	SEEME We lazily include C11 in this bucket. In practice, including any C
	headers C++-style always dumps functions directly in the std namespace,
	and so the distinction we try to make here is probably pedantic and 
	unnecessary.
*/

#if defined (__STDC_VERSION__ ) && (__STDC_VERSION__ >= 199901L)
	#define LUL_STDC99								std
#else
	//	SEEME MSVS still doesn't define this macro (for sound reasons) but it
	//	leaves us without a meaningful test here. For now, we punt and assume
	//	we'll always have availability in the std namespace directly if a
	//	given C99/C11 function is available.
	#define LUL_STDC99								std
#endif
