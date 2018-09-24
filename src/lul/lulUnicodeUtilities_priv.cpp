/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“UnicodeUtilities_priv.cpp”
	Copyright © 2010-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only


//	std
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <string_view>


//	lul
#include <Lucena-Utilities/lulTypes.hpp>
#include <Lucena-Utilities/lulUtilities.hpp>

#include "lulConfig_priv.hpp"
#include "lulUnicodeUtilities_priv.hpp"


LUL_begin_v_namespace

namespace uc {


/*------------------------------------------------------------------------------
	A simple table describing the number of bytes needed to encode different
	ranges of code points using UTF-8.

	Some properties of UTF-8 encoding:
	- ISO 10646 actually uses up to 6 bytes for UTF-8 encoding, but we
	restrict ourselves to strict Unicode, so we top out at 4.
	- Bytes with ASCII meanings cannot appear as part of a multi-byte sequence;
	all bytes in a multibyte sequence have their hi bit set (&0x80 == 0x80).
	- The first byte of a multibyte sequence is always in [0xC0, 0xFD], and the
	number of 1’s at the start of the lead byte indicate how many total bytes
	are in the sequence.
	- The second and subsequent bytes are always in [0x80, 0xBF].
	- 0xFE and 0xFF never appear in UTF-8 encoding.
	- Byte ordering is effectively Bigendian.
	- Basic Multilingual Plane (BMP) encodings never require more than 3 bytes.
	- Although there may be several ways to encode a code point, only the
	simplest one is considered valid.
*/

const char32_t
UTF8_code_point_range [4][2] =
{
	{	0x00000000u, 0x0000007Fu	},	//	0xxxxxxx
	{	0x00000080u, 0x000007FFu	},	//	110xxxxx 10xxxxxx
	{	0x00000800u, 0x0000FFFFu	},	//	1110xxxx 10xxxxxx 10xxxxxx
	{	0x00010000u, 0x0010FFFFu	}	//	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
};


/*------------------------------------------------------------------------------
	Lookup table for quickly determining how many trail bytes follow a given
	lead byte. Recall that for multibyte UTF-8 sequences, the number of 1’s
	at the start of a lead byte indicate how many total bytes are in the
	sequence. Use the lead byte as an index into the table to get back the
	number of expected trail bytes.
*/

const uint8_t
UTF8_count_trail_bytes [256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3,
    0, 0, 0,		//	3 bytes in UCS, illegal in Unicode
    0, 0, 0, 0,		//	4 bytes in UCS, illegal in Unicode
    0, 0,			//	5 bytes in UCS, illegal in Unicode
    0, 0			//	illegal bytes 0xfe and 0xff
};


/*------------------------------------------------------------------------------
	Decode one byte sequence at a time and return the decoded code point in
	out_code. We expect in_string to be valid Unicode; if not, we treat is as
	suspect and skip the illegal parts. Note that we do not make any
	assumptions about other possible character sets (e.g., UCS), and simply
	make use of UTF-8’s robustness to get back to someplace that looks like
	valid Unicode. At the end of this function, io_index will point to the
	first byte after the sequence that was processed. If we find a bad
	sequence, we burn bytes until io_index points to next good byte after the
	sequence and return false. In any case where the function returns false,
	the value of out_code is undefined, otherwise it will contain a validated
	code point.
*/

bool
UTF_Decode (
	std::string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code)
{
	LUL_Assert_ (io_index < in_string.size());

	if (io_index >= in_string.size())
	{
		//	parameter error
		return false;
	}

	auto cur = *(reinterpret_cast <const uint8_t *> (&in_string [io_index++]));

	//	Check if we’re within ASCII range; if so, we’re done.
	if (UTF8_Is_ASCII (cur))
	{
		//	[U+0000, U+007F]	0xxxxxxx	always valid
		out_code = cur;
		return true;
	}
	else
	{
		//	Make sure we have a lead byte, since this is a multibyte sequence.
		if (UTF8_Is_Lead_Byte (cur))
		{
			auto trail_count = UTF8_count_trail_bytes [cur];

			//	Make sure we have enough string left to accommodate all our
			//	supposed trail bytes.
			if (trail_count and ((io_index + trail_count) <= in_string.size()))
			{
				//	Mask off the part of the lead byte that doesn’t actually
				//	contain encoded information.
				out_code = UTF8_Decode_Lead_Byte (cur, trail_count);

				for (std::size_t i = 0; i < trail_count; ++i)
				{
					auto trail_byte = *(reinterpret_cast <const uint8_t *> (
						&in_string [io_index]));

					if (UTF8_Is_Trail_Byte (trail_byte))
					{
						out_code = UTF8_Decode_Trail_Byte (trail_byte, out_code);

						//	Only increment once we’ve consumed the valid byte.
						++io_index;
					}
					else
					{
						//	If it’s not a trail byte, it’s the start of the
						//	next sequence popping up early and unexpectedly.
						//	Fail immediately.
						return false;
					}
				}


				//	UTF-8 requires that a given code point be represented by
				//	the simplest possible sequence. We make sure our out_code
				//	meets this criteria, otherwise it’s a failure.
				if (out_code >= UTF8_code_point_range [trail_count][0])
				{
					return Is_Char (out_code);
				}
			}
		}
	}


	//	We get here whenever we encounter an illegal sequence and want to
	//	spill bytes until we get to something that looks valid before
	//	exiting. This has the effect of consolidating bad sequence bytes
	//	into a single bogus code point.
	while ((io_index < in_string.size()) and
		UTF8_Is_Trail_Byte (*(reinterpret_cast <const uint8_t *> (
			&in_string [io_index]))))
	{
		//	Note that io_index is only incremented if the byte it points to
		//	is a spurious trail byte. In all other cases, it’s left alone
		//	since the caller will probably want to process the byte as the
		//	start of the next sequence.
		++io_index;
	}

	return false;
}


/*------------------------------------------------------------------------------
	Decode one character sequence at a time and return the decoded code point in
	out_code. Code points are validated as Unicode characters; all other values
	are rejected and cause the function to return false. If false is returned,
	the value in out_code is undefined.

	Note that wchar_t arrays will be automatically handled here if wchar_t is
	16 bits.
*/

bool
UTF_Decode (
	std::u16string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code)
{
	LUL_Assert_ (io_index < in_string.size());

	if (io_index >= in_string.size())
	{
		//	parameter error
		return false;
	}


	auto char1 = in_string [io_index++];

	if (Is_Surrogate (char1))
	{
		if (Is_Lead_Surrogate (char1) and (io_index < in_string.size()))
		{
			auto char2 = in_string [io_index];

			if (Is_Trail_Surrogate (char2))
			{
				//	Valid surrogate pair. Each surrogate contributes 10 bits to
				//	the final value, which will be in Unicode supplementary
				//	space [U+10000, U+10FFFF]. 0x035FDC00u is derived from
				//	(0x0000D800u << 10) + 0x0000DC00u - 0x00010000u.
				out_code = (((char32_t) char1) << 10) + (((char32_t) char2) - 0x035FDC00u);

				//	Wait to increment io_index till the character is consumed.
				++io_index;
			}
			else
			{
				//	Invalid surrogate pair
				return false;
			}
		}
		else
		{
			//	Orphaned surrogate.
			return false;
		}
	}
	else
	{
		//	Not a surrogate, just one 16-bit word. Note that we still need to
		//	verify that this is a valid Unicode code point.
		out_code = char1;
	}

	return Is_Char (out_code);
}


/*------------------------------------------------------------------------------
	Return one code point at a time in out_code. Code points are validated as
	Unicode characters; all other values are rejected and cause the function to
	return false. If false is returned, the value in out_code is undefined.

	Note that wchar_t arrays will be automatically handled here if wchar_t is
	32 bits.
*/

bool
UTF_Decode (
	std::u32string_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code)
{
	//	A char is always consumed, so io_index is always incremented.
	return (io_index < in_string.size()) ?
		Is_Char (out_code = in_string [io_index++]) : false;
}


/*------------------------------------------------------------------------------
	Decode one character sequence at a time and return the decoded code point in
	out_code. Code points are validated as Unicode characters; all other values
	are rejected and cause the function to return false. If false is returned,
	the value in out_code is undefined.

	Note that wchar_t arrays will be automatically handled here if wchar_t is
	16 bits.
*/

bool
UTF_Decode (
	std::wstring_view		in_string,
	std::size_t &			io_index,
	char32_t &				out_code)
{
	#if LUL_TYPE_WCHAR_T_IS_16_BITS
		return UTF_Decode (std::u16string_view {
			reinterpret_cast <const typename U16String::char_type *> (in_string.data()),
				in_string.size()}, io_index, out_code);
	#else
		return UTF_Decode (std::u32string_view {
			reinterpret_cast <const typename U32String::char_type *> (in_string.data()),
				in_string.size()}, io_index, out_code);
	#endif
}


/*------------------------------------------------------------------------------
	This is a UTF-8 encoder. We assume that in_code is a valid Unicode
	character. Always append new sequences to io_string, ignoring the rest of
	the contents. Return the number of bytes written.
*/

size_t
UTF_Encode (
	char32_t				in_code,
	std::string &			io_string)
{
	LUL_Assert_ (Is_Char (in_code));

	//	ASCII short-circuit.
	if (in_code <= UTF8_code_point_range [0][1])
	{
		//	1-byte sequence in [U+0000, U+007F]
		io_string.push_back (static_cast <char> (in_code));
		return 1;
	}


	//	We’ll be writing a multi-byte sequence. To do so, we figure out how
	//	many bytes we’ll need based on the code point’s location in a range
	//	table, then we encode the trail bytes
	std::size_t out_bytes = 0;

	if (in_code <= UTF8_code_point_range [1][1])
	{
		//	2-byte sequence in [U+0080, U+07FF]
		//	Encode the lead byte with a 3-bit marker and 5 data bits.
		io_string.push_back (static_cast <char> ((in_code >> 6) | 0xC0));

		out_bytes = 2;
	}
	else
	{
		if (in_code <= UTF8_code_point_range [2][1])
		{
			//	3-byte sequence in [U+0800, U+FFFF]
			//	Encode the lead byte with a 4-bit marker and 4 data bits.
			io_string.push_back (static_cast <char> ((in_code >> 12) | 0xE0));

			out_bytes = 3;
		}
		else
		{
			//	4-byte sequence in [U+10000, U+10FFFF]
			//	Encode the lead byte with a 5-bit marker and 3 data bits.
			io_string.push_back (static_cast <char> ((in_code >> 18) | 0xF0));

			//	Encode a trail byte with a 2-bit marker and 6 data bits.
			io_string.push_back (static_cast <char> (((in_code >> 12) & 0x3F) | 0x80));

			out_bytes = 4;
		}

		//	Encode a trail byte with a 2-bit marker and 6 data bits.
		io_string.push_back (static_cast <char> (((in_code >> 6) & 0x3F) | 0x80));
	}

	//	Encode a trail byte with a 2-bit marker and 6 data bits.
	io_string.push_back (static_cast <char> ((in_code & 0x3F) | 0x80));


	return out_bytes;
}


/*------------------------------------------------------------------------------
	This is a UTF-16 encoder. We assume that in_code is a valid Unicode
	character. Always append new sequences to io_string, ignoring the rest of
	the contents. Return the number of characters (char16_t) written.
*/

size_t
UTF_Encode (
	char32_t				in_code,
	std::u16string &		io_string)
{
	LUL_Assert_ (Is_Char (in_code));

	if (Is_In_BMP (in_code))
	{
		//	The code point is in the BMP and can be encoded as 1 character.
		io_string.push_back (static_cast <char16_t> (in_code));
		return 1;
	}

	//	Non-BMP characters are encoded as a surrogate pair.
	//	Encode the lead surrogate with a 6-bit marker and 10 data bits.
	io_string.push_back (static_cast <char16_t> ((in_code >> 10) + 0xD7C0u));

	//	Encode the trail surrogate with a 6-bit marker and 10 data bits.
	io_string.push_back (static_cast <char16_t> ((in_code & 0x3FFu) | 0xDC00u));

	return 2;
}


/*------------------------------------------------------------------------------
	This is a UTF-32 “encoder”. We assume that in_code is a valid Unicode
	character, so we simply append it to io_string, and return 1.
*/

size_t
UTF_Encode (
	char32_t				in_code,
	std::u32string &		io_string)
{
	LUL_Assert_ (Is_Char (in_code));

	io_string.push_back (in_code);

	return 1;
}


/*------------------------------------------------------------------------------
	Note that we cannot safely treat a std::wstring as a u16string or
	u32string - even if they are equivalent - since the underlying char_type
	can be different, which means the template instantiation is different,
	which means we might inadvertently wreak havoc depending on what
	optimizations a given string implementation makes use of.

	This is the lazy approach; copy-and-paste from the U16String and U32String
	versions. Read those for comments.
*/

size_t
UTF_Encode (
	char32_t				in_code,
	std::wstring &			io_string)
{
	LUL_Assert_ (Is_Char (in_code));

	#if LUL_TYPE_WCHAR_T_IS_16_BITS
		if (Is_In_BMP (in_code))
		{
			io_string.push_back (static_cast <wchar_t> (in_code));
			return 1;
		}
		else
		{
			io_string.push_back (static_cast <wchar_t> ((in_code >> 10) + 0xD7C0u));
			io_string.push_back (static_cast <wchar_t> ((in_code & 0x3FFu) | 0xDC00u));
			return 2;
		}
	#else
		io_string.push_back (static_cast <wchar_t> (in_code));
		return 1;
	#endif
}


/*----------------------------------------------------------------------------*/

}	//	namespace uc

LUL_end_v_namespace

#endif	//	LUL_CONFIG_headers_only
