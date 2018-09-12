/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Types.cpp”
	Copyright © 2013-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


//	std
#include <cassert>
#include <cstdarg>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulMemoryTypes.hpp>
#include <Lucena-Utilities/lulTypes.hpp>

#include "lulConfig_priv.hpp"


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Constants
*/

constexpr int k_max_string_length {4096};


/*------------------------------------------------------------------------------
	Utility code to convert a va_list into a std::string. The new string is
	created on the stack. If string creation fails, an empty string is
	returned. This function does not throw.
*/

std::string
VA_To_String (
	const char *			in_format,
	...)
{
	try
	{
		std::va_list			argument_list;
		char					var_args[k_max_string_length];
		int						chars_written;

		va_start (argument_list, in_format);

		chars_written = LUL_STDC99::vsnprintf (
			var_args,
			k_max_string_length,
			in_format,
			argument_list);

		va_end (argument_list);

		if ((chars_written > 0) and (chars_written <= k_max_string_length))
		{
			return std::string (var_args);
		}
	}

	catch (...)
	{
		//	fall through
	}


	return std::string();
}


/*------------------------------------------------------------------------------
	static
*/

ErrorHandler *
ErrorHandler::Get (
	ErrorHandlerUnq			in_handler)
{
	static auto s_default_handler = ErrorHandlerUnq{};
	static auto s_custom_handler = ErrorHandlerUnq {nullptr};

	if (s_custom_handler)
	{
		return s_custom_handler.get();
	}
	
	if (in_handler)
	{
		s_custom_handler = std::move (in_handler);
		return s_custom_handler.get();
	}
	
	return s_default_handler.get();
}


/*------------------------------------------------------------------------------
	This function should be overridden.
*/

void
ErrorHandler::Post (
	Status					in_status)
{
	std::cerr <<
		"(Domain: " << in_status.Domain() << ", " <<
		"Code: " << in_status.Code() << "): " << std::endl <<
		in_status.Message() << std::endl <<
		in_status.File() << ", " <<
		in_status.Function() << ": "<<
		in_status.Line();
}


/*------------------------------------------------------------------------------
*/

struct U8String::Det
{
	using char_type = typename U8String::char_type;

	std::string _s;

	Det (const char_type * s) : _s {s} { }
	Det (const char_type * s, size_type n) : _s {s, n} { }
	Det (const std::string & s) : _s {s} { }
	Det (std::string && s) : _s {std::move (s)} { }
	Det (std::string_view s) : _s {s} { }
};


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	const U8String &		a)
	:	_d {std::make_unique <U8String::Det> (a._d->_s)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	U8String &&				a)
	:	_d {std::move (a._d)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	const char_type *		s)
	:	_d {std::make_unique <U8String::Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	const char_type *		s,
	size_type				n)
	:	_d {std::make_unique <U8String::Det> (s, n)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	const std::string &		s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	std::string &&			s)
	:	_d {std::make_unique <Det> (std::move (s))}
{
}


/*------------------------------------------------------------------------------
*/

U8String::U8String (
	std::string_view		s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U8String::~U8String() noexcept
{
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	const U8String &		a)
{
	_d = std::make_unique <U8String::Det> (a._d->_s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	U8String &&				a)
{
	_d = std::move (a._d);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	std::string_view		s)
{
	_d = std::make_unique <U8String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	const std::string &		s)
{
	_d = std::make_unique <U8String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	std::string &&			s)
{
	_d = std::make_unique <U8String::Det> (std::move (s));
	return *this;
}


/*------------------------------------------------------------------------------
*/

U8String &
U8String::operator = (
	const char_type *		s)
{
	_d = std::make_unique <U8String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

std::string_view
U8String::Get_StringView() const
{
	return std::string_view {_d->_s};
}


/*------------------------------------------------------------------------------
*/

auto
U8String::c_str() const -> const char_type *
{
	return _d->_s.c_str();
}


/*------------------------------------------------------------------------------
*/

struct U16String::Det
{
	using char_type = typename U16String::char_type;

	std::u16string _s;

	Det (const char_type * s) : _s {s} { }
	Det (const char_type * s, size_type n) : _s {s, n} { }
	Det (const std::u16string & s) : _s {s} { }
	Det (std::u16string && s) : _s {std::move (s)} { }
	Det (std::u16string_view s) : _s {s} { }
};


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	const U16String &		a)
	:	_d {std::make_unique <U16String::Det> (a._d->_s)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	U16String &&			a)
	:	_d {std::move (a._d)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	const char_type *		s)
	:	_d {std::make_unique <U16String::Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	const char_type *		s,
	size_type				n)
	:	_d {std::make_unique <U16String::Det> (s, n)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	const std::u16string &	s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	std::u16string &&		s)
	:	_d {std::make_unique <Det> (std::move (s))}
{
}


/*------------------------------------------------------------------------------
*/

U16String::U16String (
	std::u16string_view		s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U16String::~U16String() noexcept
{
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	const U16String &		a)
{
	_d = std::make_unique <U16String::Det> (a._d->_s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	U16String &&			a)
{
	_d = std::move (a._d);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	std::u16string_view		s)
{
	_d = std::make_unique <U16String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	const std::u16string &	s)
{
	_d = std::make_unique <U16String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	std::u16string &&		s)
{
	_d = std::make_unique <U16String::Det> (std::move (s));
	return *this;
}


/*------------------------------------------------------------------------------
*/

U16String &
U16String::operator = (
	const char_type *		s)
{
	_d = std::make_unique <U16String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

std::u16string_view
U16String::Get_StringView() const
{
	return std::u16string_view {_d->_s};
}


/*------------------------------------------------------------------------------
*/

auto
U16String::c_str() const -> const char_type *
{
	return _d->_s.c_str();
}


/*------------------------------------------------------------------------------
*/

struct U32String::Det
{
	using char_type = typename U32String::char_type;

	std::u32string _s;

	Det (const char_type * s) : _s {s} { }
	Det (const char_type * s, size_type n) : _s {s, n} { }
	Det (const std::u32string & s) : _s {s} { }
	Det (std::u32string && s) : _s {std::move (s)} { }
	Det (std::u32string_view s) : _s {s} { }
};


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	const U32String &		a)
	:	_d {std::make_unique <U32String::Det> (a._d->_s)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	U32String &&				a)
	:	_d {std::move (a._d)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	const char_type *		s)
	:	_d {std::make_unique <U32String::Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	const char_type *		s,
	size_type				n)
	:	_d {std::make_unique <U32String::Det> (s, n)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	const std::u32string &	s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	std::u32string &&		s)
	:	_d {std::make_unique <Det> (std::move (s))}
{
}


/*------------------------------------------------------------------------------
*/

U32String::U32String (
	std::u32string_view		s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

U32String::~U32String() noexcept
{
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	const U32String &		a)
{
	_d = std::make_unique <U32String::Det> (a._d->_s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	U32String &&				a)
{
	_d = std::move (a._d);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	std::u32string_view		s)
{
	_d = std::make_unique <U32String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	const std::u32string &	s)
{
	_d = std::make_unique <U32String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	std::u32string &&		s)
{
	_d = std::make_unique <U32String::Det> (std::move (s));
	return *this;
}


/*------------------------------------------------------------------------------
*/

U32String &
U32String::operator = (
	const char_type *		s)
{
	_d = std::make_unique <U32String::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

std::u32string_view
U32String::Get_StringView() const
{
	return std::u32string_view {_d->_s};
}


/*------------------------------------------------------------------------------
*/

auto
U32String::c_str() const -> const char_type *
{
	return _d->_s.c_str();
}


/*------------------------------------------------------------------------------
*/

struct WString::Det
{
	using char_type = typename WString::char_type;

	std::wstring _s;

	Det (const char_type * s) : _s {s} { }
	Det (const char_type * s, size_type n) : _s {s, n} { }
	Det (const std::wstring & s) : _s {s} { }
	Det (std::wstring && s) : _s {std::move (s)} { }
	Det (std::wstring_view s) : _s {s} { }
};


/*------------------------------------------------------------------------------
*/

WString::WString (
	const WString &			a)
	:	_d {std::make_unique <WString::Det> (a._d->_s)}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	WString &&				a)
	:	_d {std::move (a._d)}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	const char_type *		s)
	:	_d {std::make_unique <WString::Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	const char_type *		s,
	size_type				n)
	:	_d {std::make_unique <WString::Det> (s, n)}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	const std::wstring &	s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	std::wstring &&			s)
	:	_d {std::make_unique <Det> (std::move (s))}
{
}


/*------------------------------------------------------------------------------
*/

WString::WString (
	std::wstring_view		s)
	:	_d {std::make_unique <Det> (s)}
{
}


/*------------------------------------------------------------------------------
*/

WString::~WString() noexcept
{
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	const WString &			a)
{
	_d = std::make_unique <WString::Det> (a._d->_s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	WString &&				a)
{
	_d = std::move (a._d);
	return *this;
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	std::wstring_view		s)
{
	_d = std::make_unique <WString::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	const std::wstring &	s)
{
	_d = std::make_unique <WString::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	std::wstring &&			s)
{
	_d = std::make_unique <WString::Det> (std::move (s));
	return *this;
}


/*------------------------------------------------------------------------------
*/

WString &
WString::operator = (
	const char_type *		s)
{
	_d = std::make_unique <WString::Det> (s);
	return *this;
}


/*------------------------------------------------------------------------------
*/

std::wstring_view
WString::Get_StringView() const
{
	return std::wstring_view {_d->_s};
}


/*------------------------------------------------------------------------------
*/

auto
WString::c_str() const -> const char_type *
{
	return _d->_s.c_str();
}


/*------------------------------------------------------------------------------
	Status::MessageBlock
*/

struct Status::MessageBlock
{
	std::string				_message;
	const char *			_file;
	const char *			_function;
	StringGroup				_domain;
	StatusCode				_code;
	int						_line;
	
	StatusUnq				_next {};


	inline					MessageBlock (
								StringGroup				in_domain,
								StatusCode				in_code,
								const std::string &		in_message,
								const char *			in_file,
								const char *			in_function,
								int						in_line)
								:	_message {in_message},
									_file {in_file},
									_function {in_function},
									_domain {in_domain},
									_code {in_code},
									_line {in_line}
							{	}

							MessageBlock (
								const MessageBlock &	in_mb)
								:	_message {in_mb._message},
									_file {in_mb._file},
									_function {in_mb._function},
									_domain {in_mb._domain},
									_code {in_mb._code},
									_line {in_mb._line}
							{
								_next = in_mb._next ?
									std::make_unique <Status> (*in_mb._next)
										: StatusUnq{};
							}
				
							MessageBlock (MessageBlock &&) noexcept = default;

							~MessageBlock() = default;

	MessageBlock &			operator = (
								const MessageBlock &	in_mb)
							{
								_message = in_mb._message;
								_file = in_mb._file;
								_function = in_mb._function;
								_domain = in_mb._domain;
								_code = in_mb._code;
								_line = in_mb._line;
								_next = in_mb._next ?
									std::make_unique <Status> (*in_mb._next)
										: StatusUnq{};
								
								return *this;
							}
	
	MessageBlock &			operator = (MessageBlock &&) noexcept = default;
};


/*------------------------------------------------------------------------------
	Effectively defaulted; this must be here since the destructor definition
	must be here.
*/

Status::Status() noexcept
{
}


/*------------------------------------------------------------------------------
	Perform a recursive deep-copy.
*/

Status::Status (
	const Status &			other) 
	:	_det {other._det ?
			std::make_unique <MessageBlock> (*other._det) :
				std::unique_ptr <MessageBlock>{}}
{
}


/*------------------------------------------------------------------------------
	other._det will be automatically reset.
*/

Status::Status (
	Status &&				other) noexcept
	:	_det {std::move (other._det)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code, "",
			"", "", 0)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	const char *			in_message)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code, in_message,
			"", "", 0)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	const std::string &		in_message)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code, in_message,
			"", "", 0)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	std::string_view		in_message)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code,
			static_cast <std::string> (in_message), "", "", 0)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	const char *			in_message,
	const char *			in_file,
	const char *			in_function,
	int						in_line)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code, in_message,
			in_file, in_function, in_line)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	const std::string &		in_message,
	const char *			in_file,
	const char *			in_function,
	int						in_line)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code, in_message,
			in_file, in_function, in_line)}
{
}


/*------------------------------------------------------------------------------
*/

Status::Status (
	StringGroup				in_domain,
	StatusCode				in_code,
	std::string_view		in_message,
	const char *			in_file,
	const char *			in_function,
	int						in_line)
	:	_det {std::make_unique <MessageBlock> (in_domain, in_code,
			static_cast <std::string> (in_message), in_file, in_function,
				in_line)}
{
}


/*------------------------------------------------------------------------------
	This has to be defined here since it must know MessageBlock’s definition.
*/

Status::~Status()
{
}


/*------------------------------------------------------------------------------
	Perform a recursive deep-copy.
*/

Status &
Status::operator = (
	const Status &			other)
{
	_det = other._det ?
		std::make_unique <MessageBlock> (*other._det) :
			std::unique_ptr <MessageBlock>{};

	return *this;
}


/*------------------------------------------------------------------------------
	other._det will be automatically reset.
*/

Status &
Status::operator = (
	Status &&				other) noexcept
{
	_det = std::move (other._det);

	return *this;
}


/*------------------------------------------------------------------------------
	Only Status objects that represent errors can be chained; if in_status has
	no _det, we return false. However, if in_status has _det and we don’t, we
	take it over.
*/

bool
Status::Chain (
	Status					in_status) noexcept
{
	if (in_status.Success())
	{
		return false;
	}
	
	if (_det)
	{
		if (_det->_next)
		{
			return _det->_next->Chain (std::move (in_status));
		}
		else
		{
			_det->_next = std::make_unique <Status> (std::move (in_status));
		}
	}
	else
	{
		_det = std::move (in_status._det);
	}
	
	return true;
}


/*------------------------------------------------------------------------------
*/

StatusCode
Status::Code() const noexcept
{
	return _det ? _det->_code : STATUS_CODE_none;
}


/*------------------------------------------------------------------------------
*/

StringGroup
Status::Domain() const noexcept
{
	return _det ? _det->_domain : STRING_GROUP_messages;
}


/*------------------------------------------------------------------------------
	All domains other than STRING_GROUP_messages are for runtime errors,
	although a code of STATUS_CODE_none is always assumed to be safe. Note that 
	we catch signals here, too, just to be safe. Also note that we recurse the 
	entire Status chain in case there’s a failure buried under one or more 
	warnings.
*/

bool
Status::Failed() const noexcept
{
	return _det ?
		((_det->_next and _det->_next->Failed()) or
			((STATUS_CODE_none != _det->_code) and
				((STRING_GROUP_messages != _det->_domain) or
					(STATUS_CODE_signal == _det->_code)))) :
		false;
}


/*------------------------------------------------------------------------------
*/

const char *
Status::File() const noexcept
{
	return _det ? _det->_file : nullptr;
}


/*------------------------------------------------------------------------------
*/

const char *
Status::Function() const noexcept
{
	return _det ? _det->_function : nullptr;
}


/*------------------------------------------------------------------------------
*/

int
Status::Line() const noexcept
{
	return _det ? _det->_line : 0;
}


/*------------------------------------------------------------------------------
*/

const char *
Status::Message() const noexcept
{
	return _det ? _det->_message.c_str() : nullptr;
}


/*------------------------------------------------------------------------------
*/

StatusReq
Status::Next() const noexcept
{
	return (_det and _det->_next) ? StatusReq {_det->_next.get()} : StatusReq{};
}


/*------------------------------------------------------------------------------
	Anything in the STRING_GROUP_messages domain or with a code of
	STATUS_CODE_none is not a runtime error (not including STATUS_CODE_signal, 
	which is immediately fatal). Note that the absence of a details structure 
	implies an all-clear status, so that’s an automatic pass. Also note that we 
	recurse the entire Status chain in case there’s a failure buried under one 
	or more warnings.
*/

bool
Status::Passed() const noexcept
{
	return _det ?
		((!_det->_next or _det->_next->Passed()) and
		(((STRING_GROUP_messages == _det->_domain) and
			(STATUS_CODE_signal != _det->_code)) or
				(STATUS_CODE_none == _det->_code))):
		true;
}


/*------------------------------------------------------------------------------
	Signals are used to indicate failures in pre-conditions, post-conditions,
	and invariants. They should only be triggered in development builds, and
	will lead to immediate termination if not handled. Note that we recurse the 
	entire Status chain in case there’s a signal buried in there.
*/

bool
Status::Signal() const noexcept
{
	return _det ?
		((_det->_next and _det->_next->Signal()) or
		((STRING_GROUP_messages == _det->_domain) and
			(STATUS_CODE_signal == _det->_code))) :
		false;
}


/*------------------------------------------------------------------------------
	Anything in the STRING_GROUP_messages domain, except for STATUS_CODE_none
	and STATUS_CODE_signal, is a warning. These are non-fatal conditions that 
	allow the program to continue executing, and may not even be reported to 
	the end user directly. Note that we recurse the entire Status chain in case 
	there’s a warning buried in there, though this is of dubious value.
*/

bool
Status::Warning() const noexcept
{
	return _det ?
		((_det->_next and _det->_next->Warning()) or
		((STRING_GROUP_messages == _det->_domain) and
			(STATUS_CODE_none != _det->_code) and
				(STATUS_CODE_signal != _det->_code))) :
		false;
}


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
