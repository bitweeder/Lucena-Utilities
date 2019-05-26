/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Packaging.hpp”
	Copyright © 2010-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


/*------------------------------------------------------------------------------
	This is here by way of documentation. The client needs to define these
	somewhere in order to make proper use of the Lucena Utilities packaging
	support, which expects to use these preprocessor constants to customize
	itself to the client. In practice, these should be defined in an early-
	loading header - such as a prefix file - or simply passed in to the
	compiler from whatever passes for a make file.

	Note that where they are used, these string literals may be converted to
	almost any sort of text encoding, but they should all be defined using
	UTF-8.

	This file should not be modified directly.

	LUL_PACKAGING_product_name is how the product identifies itself to the
	world. The actual name may be different to adhere to the standards of the
	platform and binary type - typically by including a prefix or suffix of
	some sort - but this provides the core of that name. In some cases, the
	localized version of the product name may be completely different, but this
	string provides a starting point for translators and also indicates where
	the name is expected to be used.

	LUL_PACKAGING_product_identifier is how the product identifies itself to the
	OS, sans prefixes and suffixes. Since it may be used as the core of things
	like directory paths and file names, it’s best to pick something that only
	uses 7-bit alpha-numeric characters to ensure maximum portability. On the
	other hand, depending on where and how it’s defined, you could have quite a
	bit more flexibility.

	LUL_PACKAGING_version is the simplified version descriptor for the program.
	It should be composed of (M)ajor release number, (m)inor release number,
	(p)atch number, and (b)uild, in the form of MM[.mm[.pppp[.b[...]]]], where
	everything in []’s is optional and the number of characters indicates the
	maximum field width, except for build, which takes the form described
	below. Note that build won’t necessarily be included, even if there is a
	value assigned to LUL_PACKAGING_build. LUL_PACKAGING_version is intended to
	be a human-readable string.

	LUL_PACKAGING_build can be in any format involving alphanumeric characters
	and members of [-_.]. It may be included in LUL_PACKAGING_version to
	represent the full version identifier for a given build of the product, but
	it has no programmatic meaning. Reasonable formats include monotonically
	increasing integers for each build of a given LUL_PACKAGING_version, UUIDs
	that uniquely identify every single build ever made, and hash numbers that
	identify a given build within a version control system.

	LUL_PACKAGING_signature is a 4-character (UTF-8) value used on certain
	platforms for identification purposes, possibly for some sort of internal
	associative database or as a file identifier. It should be reasonably
	unique and may change across platforms. To be a bit obtuse, it’s only here
	for the platforms that actually need it.

	LUL_PACKAGING_copyright_holder and LUL_PACKAGING_copyright_date are combined
	with other values here for generating copyright notices in places like
	dialog boxes. Programs that have elaborate requirements in this area (e.g.,
	multiple holders over different spans of time, different rights held by
	different entities, etc.) may end up simply manually overriding the
	automatic messages wherever they appear.

	LUL_PACKAGING_product_domain is the domain (or subdomain and domain) of the
	product’s web site. This is usually the developer’s web site, as it’s used
	for generating a URI to uniquely identify the product. Note that this does
	not necessarily identify the product’s actual web page, and in fact,
	probably doesn’t. This must be in reverse DNS form, e.g., "tld.domain".

	LUL_PACKAGING_description is a simple description of the product, typically
	10 to 12 words or less.

	LUL_PACKAGING_api identifies the system-level API primarily used for this
	particular build, such as “Carbon” or “WinAPI”. This can be used for
	selecting platform-specific files for various purposes, among other things;
	the string is guaranteed to be alphanumeric and start with a letter.

	LUL_PACKAGING_prefix is a naming prefix used by files in this project.
*/

#if defined (LUL_CONFIG_debug) && LUL_CONFIG_debug
	#if !defined (LUL_PACKAGING_product_name)
		#define LUL_PACKAGING_product_name					u8"Application Debug"
	#endif	//	LUL_PACKAGING_product_name

	#if !defined (LUL_PACKAGING_product_identifier)
		#define LUL_PACKAGING_product_identifier			u8"app-d"
	#endif	//	LUL_PACKAGING_product_identifier
#else
	#if !defined (LUL_PACKAGING_product_name)
		#define LUL_PACKAGING_product_name					u8"Application"
	#endif	//	LUL_PACKAGING_product_name

	#if !defined (LUL_PACKAGING_product_identifier)
		#define LUL_PACKAGING_product_identifier			u8"app"
	#endif	//	LUL_PACKAGING_product_identifier
#endif	//	LUL_CONFIG_debug

#if !defined (LUL_PACKAGING_version)
	#define LUL_PACKAGING_version							u8"1.0.0"
#endif	//	LUL_PACKAGING_version

#if !defined (LUL_PACKAGING_build)
	#define LUL_PACKAGING_build								u8"1"
#endif	//	LUL_PACKAGING_build

#if !defined (LUL_PACKAGING_signature)
	#define LUL_PACKAGING_signature							u8"????"
#endif	//	LUL_PACKAGING_signature

#if !defined (LUL_PACKAGING_copyright_holder)
	#define LUL_PACKAGING_copyright_holder					u8"Company Name"
#endif	//	LUL_PACKAGING_copyright_holder

#if !defined (LUL_PACKAGING_copyright_date)
	#define LUL_PACKAGING_copyright_date					u8"2019"
#endif	//	LUL_PACKAGING_copyright_date

#if !defined (LUL_PACKAGING_product_domain)
	#define LUL_PACKAGING_product_domain					u8"com.example"
#endif	//	LUL_PACKAGING_product_domain

#if !defined (LUL_PACKAGING_api)
	#define LUL_PACKAGING_api								u8"API"
#endif	//	LUL_PACKAGING_api

#if !defined (LUL_PACKAGING_prefix)
	#define LUL_PACKAGING_prefix							u8""
#endif	//	LUL_PACKAGING_prefix

#if !defined (LUL_PACKAGING_description)
	#define LUL_PACKAGING_description						u8"An application."
#endif	//	LUL_PACKAGING_description
