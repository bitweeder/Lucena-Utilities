/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Utilities.hpp”
	Copyright © 1999-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <string_view>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulTypes.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Constants
*/

//	APIME This is a refugee from the private Unicode utilities (which will
//	eventually be brought into the public namespace as an optional component
//	for client builds).
namespace uc
{
	const char32_t			CODE_POINT_invalid					(~0u);
}	//	namespace uc


/*------------------------------------------------------------------------------
	Utilities

	Utilities all exist in the gs namespace.
*/

std::string_view
Get_Target_Vec_String();

bool
Verify_String (
	std::string_view		in_string);

bool
Verify_String (
	std::u16string_view		in_string);

bool
Verify_String (
	std::u32string_view		in_string);


/*------------------------------------------------------------------------------
	String Conversions

	From my dainty perch in the future, it looks like the C and C++ standard
	committees really half-assed it when it came to international string
	support. The train wreck is being slowly sorted, but for now, things are
	still hellish. And so we have this header, which is a stand-in for the real
	solutions (WG14 N1040, and WG21 N2018) that are on the horizon and which
	mimics their structure and usage.

	For our purposes, all strings are in Unicode, typically encoded as UTF-8,
	though UTF-16 and UTF-32 are possible for strings passed in from OS-level
	routines. Note that we’re completely ignoring old-style C multi-byte char
	strings, but we do still have to account for potentially multi-character
	Unicode glyphs thanks to surrogate characters; other multi-byte encodings
	are ignored.

	To be safe, we define conversions from almost everything to almost
	everything else, and then just NOP at the implementation level, as
	appropriate. Note that we return a string even if the conversion was not
	completely clean; that just means there will be Unicode replacement
	characters in the output string. out_clean indicates whether or not
	conversion was completely successful.

	It’s also worth noting that since we always try to use Unicode encodings
	for native strings through appropriate locale settings, a significant
	number of our conversion functions really are going to be NOPs; however,
	pedantry insists that conversions always be used.
	
	Finally, note that in_length always represents the number of “chars” in
	the original encoding, which is not necessarily equal to the number of
	printable characters (or semantically meaningful characters, anyway). As a
	result, the length of the resulting string will not necessarily be the 
	same, e.g., when converting between UTF-8 string that uses non-ASCII (or
	even extended ASCII) characters to UTF-32 (where every code point can be
	represented as a single “char”).

	WARNING: These can be slow if they’re not NOPs, as an unoptimized, naive 
	conversion involves translating everything first to Unicode code points and 
	then to the final encoding.
*/

LUL_v_::WString
UTF8_to_Wide (
	std::string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U16String
UTF8_to_UTF16 (
	std::string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U32String
UTF8_to_UTF32 (
	std::string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::WString
UTF16_to_Wide (
	std::u16string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U8String
UTF16_to_UTF8 (
	std::u16string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U32String
UTF16_to_UTF32 (
	std::u16string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::WString
UTF32_to_Wide (
	std::u32string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U8String
UTF32_to_UTF8 (
	std::u32string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U16String
UTF32_to_UTF16 (
	std::u32string_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U8String
Wide_to_UTF8 (
	std::wstring_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U16String
Wide_to_UTF16 (
	std::wstring_view		in_string,
	bool *					out_clean = nullptr);

LUL_v_::U32String
Wide_to_UTF32 (
	std::wstring_view		in_string,
	bool *					out_clean = nullptr);


/*------------------------------------------------------------------------------
	String Width

	Given the various char_types involved, we must differentiate between the
	different length characteristics of a string, namely:

		1. number of bytes in a string
		2. number of characters in a string
		3. number of cells in a string (e.g., display width, for monospace)

	#2 is trivially found by calling [string]::size, and #1 can be found by
	multiplying that value by sizeof ([string]::value_type).

	The third length characteristic case is dicey since there is no standard
	way to get it. For example, on POSIX platforms with extensions to
	<wchar.h>, wcwidth will give a cell count for a wide character using the
	current locale, while wcswidth will do the same for a wide string. However,
	there is no equivalent functionality under Windows, and a trivial port is
	complicated by the differing widths of wchar_t under POSIX and Windows. The
	solution is a wc[s]width-alike family of functions that accept UTF-X chars.
*/

/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
