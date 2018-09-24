/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Utilities.cpp”
	Copyright © 1999-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only


//	std
#include <string>
#include <string_view>


//	lul
#include <Lucena-Utilities/lulTypes.hpp>
#include <Lucena-Utilities/lulUtilities.hpp>

#include "lulConfig_priv.hpp"
#include "lulUnicodeUtilities_priv.hpp"


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Prototypes
*/

void Append_With_Delimiter (
	std::string_view, std::string_view, std::string);

std::string Generate_Target_Vec_String();


/*------------------------------------------------------------------------------
*/

void
Append_With_Delimiter (
	std::string_view	in_term,
	std::string_view	in_delimiter,
	std::string			io_string)
{
	if (!io_string.empty()) io_string += in_delimiter;

	io_string += in_term;
}


/*------------------------------------------------------------------------------
*/

std::string
Generate_Target_Vec_String()
{
	auto nrv = std::string{};

	if constexpr (LUL_TARGET_VEC_SSE) Append_With_Delimiter ("SSE", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_SSE2) Append_With_Delimiter ("SSE2", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_SSE3) Append_With_Delimiter ("SSE3", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_SSE41) Append_With_Delimiter ("SSE4.1", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_SSE42) Append_With_Delimiter ("SSE4.2", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_AVX) Append_With_Delimiter ("AVX", ", ", nrv);
	if constexpr (LUL_TARGET_VEC_AVX2) Append_With_Delimiter ("AVX2", ", ", nrv);

	return nrv;
}


/*------------------------------------------------------------------------------
*/

std::string_view
Get_Target_Vec_String()
{
	static auto s_vec_string = Generate_Target_Vec_String();

	return s_vec_string;
}


/*------------------------------------------------------------------------------
*/

U16String
UTF8_to_UTF16 (
	std::string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U16String {uc::To_Ustring <std::u16string> (in_string,
		uc::USTRING_CONV_wider, out_clean)};

	return nrv;
}


/*------------------------------------------------------------------------------
*/

U32String
UTF8_to_UTF32 (
	std::string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U32String {uc::To_Ustring <std::u32string> (in_string,
		uc::USTRING_CONV_wider, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

WString
UTF8_to_Wide (
	std::string_view		in_string,
	bool *					out_clean)
{
	auto nrv = WString {uc::To_Ustring <std::wstring> (in_string,
		uc::USTRING_CONV_wider, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

U8String
UTF16_to_UTF8 (
	std::u16string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U8String {uc::To_Ustring <std::string> (in_string,
		uc::USTRING_CONV_narrower, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

U32String
UTF16_to_UTF32 (
	std::u16string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U32String {uc::To_Ustring <std::u32string> (in_string,
		uc::USTRING_CONV_same, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

WString
UTF16_to_Wide (
	std::u16string_view		in_string,
	bool *					out_clean)
{
	auto nrv = WString {uc::To_Ustring <std::wstring> (in_string,
		uc::USTRING_CONV_same, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

U8String
UTF32_to_UTF8 (
	std::u32string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U8String {uc::To_Ustring <std::string> (in_string,
		uc::USTRING_CONV_narrower, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

U16String
UTF32_to_UTF16 (
	std::u32string_view		in_string,
	bool *					out_clean)
{
	auto nrv = U16String {uc::To_Ustring <std::u16string> (in_string,
		uc::USTRING_CONV_same, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
*/

WString
UTF32_to_Wide (
	std::u32string_view		in_string,
	bool *					out_clean)
{
	#if LUL_TYPE_WCHAR_T_IS_16_BITS
		auto nrv = WString {uc::To_Ustring <std::wstring> (in_string,
			uc::USTRING_CONV_same, out_clean)};
	#else
		//	We assume the incoming string is already valid Unicode.
		if (out_clean) *out_clean = true;

		auto nrv = WString {
			reinterpret_cast <const typename WString::char_type *> (in_string.data()),
				in_string.size()};
	#endif
	
	return nrv;
}


/*------------------------------------------------------------------------------
	Verify that all the encoded sequences in the string are valid for Unicode.
*/

bool
Verify_String (
	std::string_view		in_string)
{
	for (std::size_t i = 0; i < in_string.size(); ++i)
	{
		char32_t code_point;

		if (!uc::UTF_Decode (in_string, i, code_point))
		{
			return false;
		}
	}

	return true;
}


/*------------------------------------------------------------------------------
	Verify all the characters and encoded pairs referenced in the string are
	valid for Unicode.
*/

bool
Verify_String (
	std::u16string_view		in_string)
{
	for (std::size_t i = 0; i < in_string.size(); ++i)
	{
		char32_t code_point;

		if (!uc::UTF_Decode (in_string, i, code_point))
		{
			return false;
		}
	}

	return true;
}


/*------------------------------------------------------------------------------
	Verify all the code points referenced in the string are valid for Unicode.
*/

bool
Verify_String (
	std::u32string_view		in_string)
{
	for (const auto & i : in_string)
	{
		if (!uc::Is_Char (i))
		{
			return false;
		}
	}

	return true;
}


/*------------------------------------------------------------------------------
*/

U8String
Wide_to_UTF8 (
	std::wstring_view		in_string,
	bool *					out_clean)
{
	auto nrv = U8String {uc::To_Ustring <std::string> (in_string,
		uc::USTRING_CONV_narrower, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
	We still perform conversions even if wchar_t == char16_t since the wide
	string may not be Unicode-encoded and we have to guarantee that a u16string
	is.
*/

U16String
Wide_to_UTF16 (
	std::wstring_view		in_string,
	bool *					out_clean)
{
	auto nrv = U16String {uc::To_Ustring <std::u16string> (in_string,
			uc::USTRING_CONV_same, out_clean)};
	
	return nrv;
}


/*------------------------------------------------------------------------------
	We still perform conversions even if wchar_t == char32_t since the wide
	string may not be Unicode-encoded and we have to guarantee that a u32string
	is.
*/

U32String
Wide_to_UTF32 (
	std::wstring_view		in_string,
	bool *					out_clean)
{
	auto nrv = U32String {uc::To_Ustring <std::u32string> (in_string,
			uc::USTRING_CONV_same, out_clean)};
	
	return nrv;
}


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace

#endif	//	LUL_CONFIG_headers_only
