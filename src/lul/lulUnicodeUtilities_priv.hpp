/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“UnicodeUtilities_priv.hpp”
	Copyright © 2010-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <string>
#include <string_view>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulTypes.hpp>
#include <Lucena-Utilities/lulUtilities.hpp>

#include "lulConfig_priv.hpp"


LUL_begin_v_namespace

namespace uc {


/*------------------------------------------------------------------------------
	Constants
*/

const char32_t			CODE_POINT_replacement				(0x0000FFFDu);


/*------------------------------------------------------------------------------
	Conversion Types

	Select same for 8-bit <-> 8-bit or 16-/32-bit <-> 16-/32-bit conversions.
	Select narrower for 16-/32-bit -> 8-bit conversions.
	Select wider for 8-bit -> 16-/32-bit conversions.

	Note that we treat 16- and 32-bit strings equivalently because of the
	peculiarity of Unicode character encoding and its effect on string size.
*/

typedef enum
{
	USTRING_CONV_same = 1,
	USTRING_CONV_narrower,
	USTRING_CONV_wider
} UstringConv;


/*------------------------------------------------------------------------------
	Declarations
*/

bool UTF_Decode (
	std::wstring_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code);

bool UTF_Decode (
	std::string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code);

bool UTF_Decode (
	std::u16string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code);

bool UTF_Decode (
	std::u32string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code);

size_t UTF_Encode (
	char32_t				in_code,
	std::wstring &			io_string);

size_t UTF_Encode (
	char32_t				in_code,
	std::string &			io_string);

size_t UTF_Encode (
	char32_t				in_code,
	std::u16string &		io_string);

size_t UTF_Encode (
	char32_t				in_code,
	std::u32string &		io_string);


/*------------------------------------------------------------------------------
	Estimate how much space to reserve in the destination string in order to
	contain the converted source string. out_string is expected to be a UTF-8
	string type (such as a std::string or a u8string, or a U8String wrapper),
	while in_string can be any UTF-16 or UTF-32 encoded C-string.

	The estimates are very rough, but should still be good enough to prevent
	unnecessary memory churn. The basic assumption is that a string likely made
	of all ASCII characters will map one-to-one, while any other string will
	require around three UTF-8 characters to represent each code point in the
	original.
*/

template <typename T, typename U>
void Ustring_Reserve_Narrow (
	const T in_string,
	U & out_string)
{
	auto len = in_string.size();
	
	out_string.clear();

	if (0 == len) return;

	out_string.reserve ((*((unsigned char *) &in_string [0]) < 0x80) ? len : (3 * len));
}

//	Instantiate known usages. Note that (wchar_t *) will map to one of these.
//template void Ustring_Reserve_Narrow (const char16_t *, std::size_t, U8String &);
//template void Ustring_Reserve_Narrow (const char32_t *, std::size_t, U8String &);


/*------------------------------------------------------------------------------
	Estimate how much space to reserve in the destination string in order to
	contain the converted source string. out_string is expected to be a UTF-16
	or UTF-32 string type (either a std::wstring, a u16string, or a u32string,
	or a wrapper such as U16String, U32String, or WString), while in_string can
	be any UTF-8 encoded string. Note that we don’t bother estimating for
	32-to-16 or 16-to-32 conversions since the vast majority of real strings
	will simply map one-to-one.

	The estimates are very rough, but should still be good enough to prevent
	unnecessary memory churn. The basic assumption is that a string likely made
	of all ASCII characters will map one-to-one, while any other string will
	require around half as many wide chars as there were UTF-8 chars.
*/

template <typename T, typename U>
void Ustring_Reserve_Wide (
	const T in_string,
	U & out_string)
{
	auto len = in_string.size();
	
	out_string.clear();

	if (0 == len) return;

	out_string.reserve ((*((unsigned char *) &in_string [0]) < 0x80) ? len : (len >> 1));
}

//	Instantiate known usages.
//template void Ustring_Reserve_Wide (const char *, std::size_t, WString &);
//template void Ustring_Reserve_Wide (const char *, std::size_t, U16String &);
//template void Ustring_Reserve_Wide (const char *, std::size_t, U32String &);


/*------------------------------------------------------------------------------
	Convert from a Unicode character source with support for basic string
	functions, such as a std::string or std::string_view, to a Unicode-encoded 
	std::basic_string, presumably with a different char-type. An invalid source 
	character is converted to a Unicode replacement character and causes the 
	function to return false; otherwise, true is returned. Note that 
	in_conv_type is used as a small optimization to determine how much space to 
	allocate for out_string.
*/

template <typename R, typename T>
R
To_Ustring (
	const T &				in_string,
	UstringConv				in_conv_type,
	bool *					out_clean)
{
	if (out_clean) *out_clean = true;


	//	Allocate our return string on the stack, and reserve space for it
	//	based on a conservative estimate.
	auto nrv = R{};
	auto len = in_string.size();

	switch (in_conv_type)
	{
		case USTRING_CONV_narrower:
		{
			Ustring_Reserve_Narrow (in_string, nrv);
			break;
		}

		case USTRING_CONV_wider:
		{
			Ustring_Reserve_Wide (in_string, nrv);
			break;
		}

		default:
		{
			nrv.reserve (len);
			break;
		}
	}


	//	Note that index is automatically advanced by UTF_Decode
	std::size_t index = 0;

	while (index < len)
	{
		char32_t code_point;

		if (UTF_Decode (in_string, index, code_point))
		{
			(void) UTF_Encode (code_point, nrv);
		}
		else
		{
			//	The decoded sequence is invalid, so we throw out any partial
			//	results and just write out the Unicode replacement character.
			//	Note that orphaned characters in a sequence are consolidated
			//	into one replacement character.
			(void) UTF_Encode (CODE_POINT_replacement, nrv);

			//	Once out_clean gets set to false, it stays that way.
			if (out_clean) *out_clean = false;
		}
	}


	return nrv;
}


/*------------------------------------------------------------------------------
	Everything after this is an inline function.
*/

/*------------------------------------------------------------------------------
	Return true if the code point lies in the Basic Multilingual Plane (BMP)
	[U+0, U+FFFF].
*/

inline bool Is_In_BMP (
	char32_t				cp)
{
	return (cp < 0x00010000u);
}


/*------------------------------------------------------------------------------
	Unicode noncharacters are code points within the Unicode range that are
	never used in character encoding. These values may have special meanings in
	other contexts (e.g., 0xfffe is used as the Byte Order Marker (BOM)).

	Code points that are noncharacters include:
		- the last two code points on each plane (U+__FFFE, U+__FFFF)
		- [U+FDD0, U+FDEF]
*/

inline bool Is_Nonchar (
	char32_t				cp)
{
	return (
		(cp >= 0xFDD0u) &&
		((cp <= 0xFDEFu) || ((cp & 0xFFFEu) == 0xFFFEu)) &&
		(cp <= 0x10FFFFu));
}


/*------------------------------------------------------------------------------
	Code points that are not valid characters include:
		- single surrogate code points [U+D800, U+DFFF]
		- the last two code points on each plane (U+__FFFE, U+__FFFF)
		- [U+FDD0, U+FDEF]
		- values outside the Unicode range [U+0000, U+10FFFF]
*/

inline bool Is_Char (
	char32_t				cp)
{
	return (
		(cp < 0xD800u) ||
		((cp > 0xDFFFu) &&
		(cp <= 0x10FFFFu) &&
		!Is_Nonchar (cp)));
}


/*------------------------------------------------------------------------------
	Unicode surrogates are in the range [U+D800, U+DFFF].
*/

template <typename CHAR_T>
inline bool Is_Surrogate (
	CHAR_T					cp)
{
	return ((static_cast <uint32_t> (cp) & 0xFFFFF800u) == 0x0000D800u);
}


/*------------------------------------------------------------------------------
	Unicode lead surrogates are in the range [U+D800, U+DBFF].
*/

template <typename CHAR_T>
inline bool Is_Lead_Surrogate (
	CHAR_T					cp)
{
	return ((static_cast <uint32_t> (cp) & 0xFFFFFC00u) == 0x0000D800u);
}


/*------------------------------------------------------------------------------
	Unicode trail surrogates are in the range [U+DC00, U+DFFF].
*/

template <typename CHAR_T>
inline bool Is_Trail_Surrogate (
	CHAR_T					cp)
{
	return ((static_cast <uint32_t> (cp) & 0xFFFFFC00u) == 0x0000DC00u);
}


/*------------------------------------------------------------------------------
	ASCII bytes are in the range [0x00, 0x7F].
*/

inline bool UTF8_Is_ASCII (
	uint8_t					b)
{
	return ((b & 0x80) == 0x00);
}


/*------------------------------------------------------------------------------
	UTF-8 lead bytes are in the range [0xC0, 0xFD].
*/

inline bool UTF8_Is_Lead_Byte (
	uint8_t					b)
{
	return ((b ^ 0x3E) > 0xC1);
}


/*------------------------------------------------------------------------------
	UTF-8 trail bytes are in the range [0x80, 0xBF].
*/

inline bool UTF8_Is_Trail_Byte (
	uint8_t					b)
{
	return (b & 0xC0) == 0x80;
}


/*------------------------------------------------------------------------------
	Mask off the part of a lead byte that doesn’t actually contain encoded
	information. tc is the number of trail bytes.
*/

inline char32_t UTF8_Decode_Lead_Byte (
	uint8_t					b,
	std::size_t				tc)
{
	return (b & ((1 << (6 - tc)) - 1));
}


/*------------------------------------------------------------------------------
	Mask off the part of a trail byte that doesn’t actually contain encoded
	information; since all trail bytes have the form (b10xxxxxx), we mask out
	the top two bits to get the real data. Slide the data already in cp over by
	6 bits, and insert the new 6 bits of data from the trail byte.
*/

inline char32_t UTF8_Decode_Trail_Byte (
	uint8_t					b,
	char32_t				cp)
{
	return (cp << 6) | (b & 0x3f);
}


/*----------------------------------------------------------------------------*/

}	//	namespace uc

LUL_end_v_namespace
