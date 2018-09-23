/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Types.hpp”
	Copyright © 2007-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	FIXME Most of this can be factored out or replaced with Standard Library
	types; there’s a lot of dated material here.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <cfloat>
#include <cmath>
#include <cstdio>

#include <deque>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <mutex>
#include <Lucena-Utilities/lulOptionalWrapper.hpp>
#include <ratio>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>
#include <Lucena-Utilities/lulTime.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Declarations
*/

class ErrorHandler;
class Status;


/*------------------------------------------------------------------------------
	Type Definitions
*/

using StringGroup = uint32_t;
using StatusCode = uint32_t;

using ErrorHandlerUnq = std::unique_ptr <ErrorHandler>;


/*------------------------------------------------------------------------------
	Error Constants
*/

constexpr StringGroup STRING_GROUP_messages	{0UL};

constexpr StatusCode STATUS_CODE_none		{0UL};
constexpr StatusCode STATUS_CODE_signal		{std::numeric_limits <StatusCode>::max()};


/*------------------------------------------------------------------------------
	ErrorHandler

	Posting a Status message involves turning the Status argument over to the
	ErrorHandler. The default behavior is to make a copy of the argument; if
	the caller has no further need of it, it is fine to move the original,
	instead, at the call site.
*/

class ErrorHandler
{
	public:
		static
		ErrorHandler *			Get (
									ErrorHandlerUnq in_handler =
										ErrorHandlerUnq {nullptr});


		virtual void			Post (Status);
};


/*------------------------------------------------------------------------------
	String Types

	Assume that strings are always Unicode-encoded. In actuality, that’s only
	true if __STDC_UTF_16__ and __STDC_UTF_32__ are defined, and the current
	locale is set to UTF-8. We make an effort to guarantee that this assumption
	remains valid.

	The types here are effectively string_views with storage; they wrap the
	equivalent std::basic_string types. Wrappers are used because it’s unsafe 
	to operate on std::basic_strings that have been passed across module 
	boundaries since there’s no guarantee that the local string implementation 
	will match that of the string source. The string wrappers deal with this by 
	providing opaque objects which can be handed off to other functions or 
	processed locally. To work with one, call its Get_StringView() member 
	function and use the string_view it returns.

	Note that while UXXString would seem to be much happier as a template
	family, that would defeat the purpose since the std::string implementation
	would then depend on the call site, meaning that the C-string functions
	would no longer be guaranteed to always work. This way, the implementation
	is always available, so we can offer a minimum functionality guarantee.

	Finally, note that the use of a unique_ptr member means that we have to
	explicitly define the UXXString destructor or provide Det as a complete
	type. Once again, the special member function cascade rears its ugly head:
	we’re forced to explicitly define everything because we define the
	destructor. More specifically, we can’t default anything since all the
	definitions have to be in the same place.

	VERME There is a minor potential issue here since we grab our type aliases
	for the wrappers from the types we’re wrapping; if a client implementation
	uses a different type than the base implementation, there will be a
	problem. In practice, this is almost definitely not worth worrying about.

	SEEME C++17, Fundamentals TS, Filesystem TS - Places we use these should
	often be re-worked to use string_views. It can be a bit annoying to do
	because of the (current) lack of direct support in std::filesystem for
	string_views; this can be worked around by switching to iterators for path
	construction from strings. Note that for instances where we can’t use
	string_views, e.g., returning strings, we may be able to simply rely on
	std::strings since their implementations have been normalized a bit in
	C++17 (e.g., the requirement to eliminate the small string optimization in
	favor of improved concurrency support).
*/

class U8String
{
	public:
		using char_type = typename std::string::value_type;
		using size_type = typename std::string::size_type;

								U8String (const char_type * = u8"");
								U8String (const U8String &);
								U8String (U8String &&);
								U8String (const char_type *, size_type);
								U8String (const std::string &);
								U8String (std::string &&);
								U8String (std::string_view);
								~U8String() noexcept;

		U8String &				operator = (const U8String &);
		U8String &				operator = (U8String &&);
		U8String &				operator = (std::string_view);
		U8String &				operator = (const std::string &);
		U8String &				operator = (std::string &&);
		U8String &				operator = (const char_type *);

		std::string_view		Get_StringView() const;
		const char_type *		c_str() const;


	private:
		struct Det;
		std::unique_ptr <Det>	_d;
};

class U16String
{
	public:
		using char_type = typename std::u16string::value_type;
		using size_type = typename std::u16string::size_type;

								U16String (const char_type * = u"");
								U16String (const U16String &);
								U16String (U16String &&);
								U16String (const char_type *, size_type);
								U16String (const std::u16string &);
								U16String (std::u16string &&);
								U16String (std::u16string_view);
								~U16String() noexcept;

		U16String &				operator = (const U16String &);
		U16String &				operator = (U16String &&);
		U16String &				operator = (std::u16string_view);
		U16String &				operator = (const std::u16string &);
		U16String &				operator = (std::u16string &&);
		U16String &				operator = (const char_type *);

		std::u16string_view		Get_StringView() const;
		const char_type *		c_str() const;


	private:
		struct Det;
		std::unique_ptr <Det>	_d;
};

class U32String
{
	public:
		using char_type = typename std::u32string::value_type;
		using size_type = typename std::u32string::size_type;

								U32String (const char_type * = U"");
								U32String (const U32String &);
								U32String (U32String &&);
								U32String (const char_type *, size_type);
								U32String (const std::u32string &);
								U32String (std::u32string &&);
								U32String (std::u32string_view);
								~U32String() noexcept;

		U32String &				operator = (const U32String &);
		U32String &				operator = (U32String &&);
		U32String &				operator = (std::u32string_view);
		U32String &				operator = (const std::u32string &);
		U32String &				operator = (std::u32string &&);
		U32String &				operator = (const char_type *);

		std::u32string_view		Get_StringView() const;
		const char_type *		c_str() const;


	private:
		struct Det;
		std::unique_ptr <Det>	_d;
};

class WString
{
	public:
		using char_type = typename std::wstring::value_type;
		using size_type = typename std::wstring::size_type;

								WString (const char_type * = L"");
								WString (const WString &);
								WString (WString &&);
								WString (const char_type *, size_type);
								WString (const std::wstring &);
								WString (std::wstring &&);
								WString (std::wstring_view);
								~WString() noexcept;

		WString &				operator = (const WString &);
		WString &				operator = (WString &&);
		WString &				operator = (std::wstring_view);
		WString &				operator = (const std::wstring &);
		WString &				operator = (std::wstring &&);
		WString &				operator = (const char_type *);

		std::wstring_view		Get_StringView() const;
		const char_type *		c_str() const;


	private:
		struct Det;
		std::unique_ptr <Det>	_d;
};


/*------------------------------------------------------------------------------
	Status

	All special member functions are defined out-of-line since Status is an
	incomplete type without a definition for MessageBlock.

	APIME We don’t use default arguments for the constructors since the
	internal representation of MessageBlock may differ depending on the
	number and type of arguments; it’s more efficient to select from different
	signatures at compile-time than make comparisons at runtime.
*/

class Status;

using StatusUnq = std::unique_ptr <Status>;
using StatusReq = LUL_::stdproxy::optional <Status *>;

class Status
{
	public:
		//	constructors
								Status() noexcept;
								Status (const Status &);
								Status (Status &&) noexcept;
								Status (
									StringGroup				in_domain,
									StatusCode				in_code);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									const char *			in_message);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									const std::string &		in_message);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									std::string_view		in_message);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									const char *			in_message,
									const char *			in_file,
									const char *			in_function,
									int						in_line);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									const std::string &		in_message,
									const char *			in_file,
									const char *			in_function,
									int						in_line);

								Status (
									StringGroup				in_domain,
									StatusCode				in_code,
									std::string_view		in_message,
									const char *			in_file,
									const char *			in_function,
									int						in_line);

								~Status();


		//	assignment
		Status &				operator = (const Status &);
		Status &				operator = (Status &&) noexcept;


		//	accessors
		StringGroup				Domain() const noexcept;
		StatusCode				Code() const noexcept;
		const char *			File() const noexcept;
		const char *			Function() const noexcept;
		int						Line() const noexcept;
		const char *			Message() const noexcept;

		StatusReq				Next() const noexcept;


		//	tests
		inline bool				Success() const noexcept	{ return !_det; }
		inline bool				Error() const noexcept		{ return !!_det; }
		bool					Warning() const noexcept;
		bool					Passed() const noexcept;
		bool					Failed() const noexcept;
		bool					Signal() const noexcept;


		//	observers
								operator bool() const noexcept
								{
									return Success();
								}


		//	actions
		bool					Chain (Status) noexcept;


	private:
		struct MessageBlock;

		std::unique_ptr <MessageBlock> _det { };
};


/*------------------------------------------------------------------------------
	StatusException

	A StatusException is intended to take ownership of a Status variable.
	Subsequent attempts to access the captured Status should refer to
	Get_Status(), which will operate on a reference to it, with all the
	expected consequences for lifetime management. Alternatively,
	Extract_Status() may be used to pass ownership of the Status variable
	out of the StatusException, resetting the stored variable to the default.

	StatusExceptions are primarily useful for dealing with complex error
	handling scenarios within a function and for passing Status between threads
	using exception_ptrs.
*/

class StatusException
	:	public std::exception
{
	private:
		Status					_status{};


	public:
								StatusException() noexcept = default;
								StatusException (const StatusException &) = default;
								StatusException (StatusException &&) noexcept = default;

								StatusException (
									Status					in_status)
										:	_status {std::move (in_status)}
								{
								}


								~StatusException() = default;

		StatusException &		operator = (const StatusException &) = default;
		StatusException &		operator = (StatusException &&) = default;


		virtual const char *	what() const throw()
								{
									return _status.Message();
								}

		Status &				Get_Status()
								{
									return _status;
								}

		Status					Extract_Status()
								{
									auto nrv = std::move (_status);

									_status = Status{};
									return nrv;
								}
};


/*------------------------------------------------------------------------------
	TSingleton

	This helper class protects unique resources from accidental deletion while
	at the same time ensuring they can only be created once. To work properly,
	all public base classes must have a protected non-virtual destructor (the
	non-virtual part is just to reinforce that polymorphic deletion is
	disallowed; it’s the protected  part that is important). To use TSingleton,
	pass in the most derived class as a template parameter and call the Get
	function. Note that raw pointers are used to avoid the overhead of
	accessing shared pointers and since there’s no danger of losing ownership.

	A TSingleton can only be created through its corresponding Get(), and
	self-destructs at program termination. Copying is disabled, but moving is
	possible.
*/

template <class T>
class TSingleton
	:	public T
{
	private:
		using Deleter = std::default_delete <TSingleton <T>>;
		friend Deleter;

								TSingleton() = default;
								TSingleton (const TSingleton &) = delete;
								~TSingleton() = default;
		TSingleton &			operator = (const TSingleton &) = delete;


	public:
								TSingleton (TSingleton &&) = default;
		TSingleton &			operator = (TSingleton &&) = default;

		//	This initialization is thread-safe in C++11.
		static T *				Get()
		{
			static std::unique_ptr <
				TSingleton <T>, Deleter> obj {new TSingleton <T>()};

			return obj.get();
		}
};


/*------------------------------------------------------------------------------
	TThreadSingleton

	This behaves identically to TSingleton except that the Singleton produced
	is unique to each thread, as opposed to being globally unique.
*/

template <class T>
class TThreadSingleton
	:	public T
{
	private:
		using Deleter = std::default_delete <TThreadSingleton <T>>;
		friend Deleter;

								TThreadSingleton() = default;
								TThreadSingleton (const TThreadSingleton &) = delete;
								~TThreadSingleton() = default;
		TThreadSingleton &		operator = (const TThreadSingleton &) = delete;


	public:
								TThreadSingleton (TThreadSingleton &&) = default;
		TThreadSingleton &		operator = (TThreadSingleton &&) = default;

		//	This initialization is thread-safe in C++11.
		static T *				Get()
		{
			thread_local std::unique_ptr <
				TThreadSingleton <T>, Deleter> obj {new TThreadSingleton <T>()};

			return obj.get();
		}
};


/*------------------------------------------------------------------------------
	TID

	Simple class to tag IDs and ensure they’re not being misused. TIDs are
	broken up into two ranges: the “logical” range is comprised of all negative
	values, while the “actual” range is comprised of all natural numbers. By
	convention, -1 is always an invalid TID. Generally, the logical range is
	fixed, and some sort of dictionary is used to map a given logical ID to an
	actual ID (or invalid_value). By contrast, the actual range may fluctuate
	as associated objects are created or destroyed. These behaviors are not
	reinforced, however; they only represent typical usage.

	Comparators are provided as members for efficiency, and provided at all
	(beyond equality/inequality) so that TIDs can be used as keys in
	associative containers. Note that requiring _pT to be a signed integral
	type guarantees a strict ordering. A conversion operator is provided so
	that TIDs can be serialized or used in switch statements; it is implicit
	since there isn’t any particular reason for it not to be. Similarly, a
	conversion constructor from std::size_t is provided as a convenience to
	simplify using TIDs as indices; it is explicit to prevent unwanted
	conversions.

	The postfix increment operator only provided for use by TIDPools; it
	returns the next actual ID after the ID provided. It is implied that only
	actual IDs will be provided as an argument; lack of enforcement of this
	rule is one reason why use of the increment operator is protected.
*/

template <class _pT> class TIDPool;

template <typename _pTag, typename _pT = int32_t, typename _pMax =
	std::integral_constant <_pT, std::numeric_limits <_pT>::max()>>
class TID
{
	static_assert (
		std::is_integral <_pT>::value &&
		std::is_signed <_pT>::value,
		"_pT must be a signed integral type");


	public:
		using tag_type = _pTag;
		using base_type = _pT;
		using invalid_value = std::integral_constant <base_type, -1>;
		using first_value = std::integral_constant <base_type, 0>;
		using last_value = _pMax;

		static_assert (
			last_value::value > first_value::value,
			"the valid range must have at least 1 value");

		static_assert (
			(invalid_value::value < first_value::value) or
			(invalid_value::value > last_value::value),
			"invalid_value cannot lie within the valid range");


		inline
		constexpr				TID() noexcept = default;

		explicit inline
		constexpr				TID (
									base_type				in_id) noexcept
									:	_id {in_id}
								{ }

		explicit inline
		constexpr				TID (
									std::size_t				in_val) noexcept
									:	_id {static_cast <base_type> (in_val &
											static_cast <std::size_t> (
												std::numeric_limits <
													base_type>::max()))}
								{ }

		inline
		constexpr bool			is_valid() const noexcept
								{
									return _id != invalid_value::value;
								}

		inline
		constexpr bool			is_first() const noexcept
								{
									return _id == first_value::value;
								}

		inline
		constexpr bool			is_last() const noexcept
								{
									return _id == last_value::value;
								}

		inline
		constexpr bool			is_actual() const noexcept
								{
									return _id >= first_value::value;
								}

		inline
		constexpr bool			is_logical() const noexcept
								{
									return _id <= invalid_value::value;
								}

		inline
		constexpr				operator base_type() const noexcept
								{
									return _id;
								}

		inline
		constexpr bool			operator == (
									const TID & rhs) const noexcept
								{
									return _id == rhs._id;
								}

		inline
		constexpr bool			operator != (
									const TID & rhs) const noexcept
								{
									return _id != rhs._id;
								}

		inline
		constexpr bool			operator < (
									const TID & rhs) const noexcept
								{
									return _id < rhs._id;
								}

		static inline
		constexpr TID			invalid() noexcept
								{
									return TID {invalid_value::value};
								}

		static inline
		constexpr TID			first() noexcept
								{
									return TID {first_value::value};
								}

		static inline
		constexpr TID			last() noexcept
								{
									return TID {last_value::value};
								}


	protected:
		friend TIDPool <TID>;

		inline
		TID						operator ++ (int) noexcept
								{
									return TID {_id++};
								}


	private:
		base_type				_id {invalid_value::value};
};


/*------------------------------------------------------------------------------
	TIDPool

	Probably unnecessary, but it increases overall robustness fairly cheaply at
	the cost of a tiny deque per pool. This allows a client program to pretty
	much run forever without running out of IDs - or at least, not before some
	other sort of resource exhaustion.

	Only non-negative integers are allocated. Only non-negative numbers can be
	freed. Calling free invalidates its argument. The last actual ID in a pool
	is reserved for use as a sentinel.

	Note that a TIDPool should only be accessed by its owner in a thread-safe
	way; no synchronization is done by the TIDPool itself.
*/

template <class N>
class TIDPool
{
	static_assert (
		std::is_same <N, TID <typename N::tag_type,
			typename N::base_type>>::value,
		"N must be a specialization of TID");


	public:
		using id_type = N;


		id_type					alloc_id()
								{
									if (_free_ids.empty())
									{
										return (_next_id == _last_id) ?
											id_type::invalid() : _next_id++;
									}
									else
									{
										auto temp = _free_ids.back();

										_free_ids.pop_back();
										return temp;
									}
								}

		void					free_id (
									id_type &				io_id)
								{
									if (io_id.is_valid())
									{
										_free_ids.push_back (io_id);
									}

									io_id = id_type::invalid();
								}

		void					clear() noexcept
								{
									_free_ids.clear();
									_next_id = id_type::first();
								}

		bool					set_last (
									const id_type &			in_id) noexcept
								{
									if ((in_id <= id_type::last()) and
										(in_id > id_type::first()))
									{
										_last_id = in_id;
										return true;
									}
									else
									{
										return false;
									}
								}

		inline bool				is_valid (
									const id_type &			in_id) const noexcept
								{
									return in_id.is_actual() and
										(in_id < _last_id);
								}


	private:
		id_type					_next_id {id_type::first()};
		id_type					_last_id {id_type::last()};
		std::deque <id_type>	_free_ids { };
};


/*------------------------------------------------------------------------------
	TTransactionID

	Simple class to represent Transaction IDs and ensure they’re not being
	misused. TTransactionIDs may be compared for equality, but are not intended
	to be used as keys or to be serialized. A TTransactionID is issued by a
	corresponding TTransactor.
*/

template <typename _pTag, typename _pT = uint32_t>
class TTransactionID
{
	static_assert (
		std::is_integral <_pT>::value,
		"_pT must be an integral type");

	public:
		using tag_type = _pTag;
		using base_type = _pT;
		using first_value = std::integral_constant <base_type, 0>;
		using last_value = std::integral_constant <base_type,
			std::numeric_limits <base_type>::max()>;


		inline
		constexpr				TTransactionID() noexcept = default;

		explicit inline
		constexpr				TTransactionID (
									base_type				in_tid) noexcept
									:	_tid {in_tid}
								{ }

		inline
		constexpr bool			operator == (
									const TTransactionID & rhs) const noexcept
								{
									return _tid == rhs._tid;
								}

		inline
		constexpr bool			operator != (
									const TTransactionID & rhs) const noexcept
								{
									return _tid != rhs._tid;
								}


	private:
		base_type				_tid {first_value::value};
};


/*------------------------------------------------------------------------------
	TTransactor

	Simple class to track Transaction IDs and ensure they’re not being
	misused. A TTransactor issues TTransactionIDs. It is assumed that the
	TTransactor is protected by a mutex, if such a thing is necessary.

	A TTransactor is initialized with the first available TTransactionID. The
	current ID can always be retrieved with Get_ID(). Calling Transact_One()
	will update the current ID to the next available ID and return this new
	value. Calling Transact_Many() will allocate a block of IDs, returning all
	of them in a std::vector; in this case, the new current ID will be the last
	one allocated. IDs issued by a given TTransactor are effectively unique
	among others issued by that TTransactor; they are very likely to overlap
	those issued by a different TTransactor. Best practice for ownership and
	usage of a TTransactor depends upon requirements for concurrency protection
	and space restrictions. A central authority makes sense when managing
	multitudes of Objects, but tracking state changes for a given Object might
	be better served by a local TTransactor, just using Get_ID() rather than
	caching the result of each call to Transact_One() (though still calling
	Transact_One() every time the state changes). Regardless, the TTransactor
	should be hosted in as non-contentious a way as conditions support, since
	it may impose burdensome synchronization requirements.

	Once all available IDs have been used (which is really unlikely for 32-bit
	or larger IDs), IDs will be recycled (which introduces the fantastically
	unlikely prospect of ID collisions). Note that if a Block is requested for
	more IDs than can be filled contiguously, a Block will be returned with
	noncontiguous IDs; in practice, this should not matter to clients. Always
	check the size() of the returned Block to see if it matches in_size; non-
	matching sizes provide the only form of error-checking this scheme
	supports.
*/

template <class _pT>
class TTransactor
{
	static_assert (
		std::is_same <_pT, TTransactionID <typename _pT::tag_type,
			typename _pT::base_type>>::value,
		"_pT must be a specialization of TTransactionID");


	public:
		using value_type = _pT;
		using block_type = std::vector <value_type>;

		value_type
		Get_ID() const noexcept
		{
			return value_type {_current};
		}

		value_type
		Transact_One() noexcept
		{
			auto id = (value_type::last_value::value > _current) ? ++_current :
				(_current = value_type::first_value::value);

			return value_type {id};
		}

		block_type
		Transact_Many (std::size_t in_size)
		{
			auto nrv = block_type{};

			if (std::size_t {value_type::last_value::value -
				value_type::first_value::value} <= in_size)
			{
				nrv.reserve (in_size);

				while (in_size)
				{
					auto id = (value_type::last_value::value > _current) ? ++_current :
						(_current = value_type::first_value::value);

					nrv.push_back (value_type {id});
					--in_size;
				}
			}

			return nrv;
		}


	private:
		using base_type = typename value_type::base_type;

		base_type _current {value_type::first_value::value};
};


/*------------------------------------------------------------------------------
	less implementation that compares dereferenced values instead of pointers;
	for use with associative containers of pointers that accept std::less as a
	template argument.
*/

struct PtrLess
{
	template <typename T>
	bool operator() (T lhs, T rhs) const
	{
		return *lhs < *rhs;
	}
};


/*------------------------------------------------------------------------------
	Free Functions
*/

std::string
VA_To_String (
	const char *			in_format,
	...);


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace


/*------------------------------------------------------------------------------
	Macros

	The use of the ‘!!’ construct for boolean conversions in various places is
	intentional and occasionally necessary.

	__VA_ARGS__ expansion trick courtesy of Richard Hansen on Stack Overflow:
	the way it works is if there’s only one argument, __VA_ARGS__ expands to
	nothing. If there is more than one argument, it expands to a comma followed
	by everything but the first argument. Only up to 9 arguments are supported,
	but that number can be trivially expanded. Note that this is only needed if
	the preprocessor doesn’t support the gcc comma-dropping trick for single-
	and empty-argument __VA_ARGS__.

	SEEME Note that we’re currently just relying on the gcc trick since all our
	compilers seem to support it and it’s easier on the eyes. Possibly faster
	to compile, too.

	APIME Macros involving Status are made trickier since it’s a type that
	is designed for moves; unintionally copying it is easy and may lead to
	performance or memory issues at really bad times. Care is taken here to
	treat parameters that resolve to a Status as & references, if possible,
	or as && references whose object gets moved into a local variable,
	otherwise. The copying issue can be resolved easily by disabling copying
	for Status; this would eliminate a surprising behavior, but may introduce
	an unnecessarily odious restriction. The reference issue is related to the
	fact that Status can have a payload and can’t be treated as a light type,
	although it usually is; payload lifetime has to be managed somehow, and
	managing it locally is the easiest way. One alternative to using
	unique_ptrs to contain the payload is to simply use shared_ptrs; these
	introduce their own kind of complexity - albeit invisible to the user -
	and also create a potential ABI issue, since shared_ptr implementations
	may differ across library boundaries. A related solution is to just
	reimplement shared_ptrs, but it doesn’t fully solve the ABI issue since our
	own implementation may change - though at least we can guarantee that
	certain operations, namely boolean conversions, won’t ever change. Another
	way to go is to have some Manager handle Status lifetimes and then just let
	the user pass around raw pointers. This is attractive in a way, but the
	danger is that we’d just end up duplicating most of the machinery of
	shared_ptrs (retain counts or similar lifetime management to determine when
	it’s safe to delete a Status payload) only spread around. For now, we’ll
	just go with the current implementation and see what issues crop up in
	practice.
*/

//	Helper Macros
#define LUL_Code_Stamp_															\
	__FILE__, __func__, __LINE__

#define LUL_Make_Status_(group_, id_, ...)										\
	LUL_v_::Status (group_, id_, LUL_v_::VA_To_String ("" __VA_ARGS__),			\
		LUL_Code_Stamp_)

#define LUL_First_Argument_(...) 												\
	LUL_First_Argument_Helper_ (__VA_ARGS__, _pIgnoreMe_)

#define LUL_First_Argument_Helper_(_pFirst_, ...) (_pFirst_)

#define LUL_Rest_of_Arguments_(...)												\
	LUL_Rest_of_Arguments_Helper_ (LUL_Number_of_Arguments_ (__VA_ARGS__),		\
		__VA_ARGS__)

#define LUL_Rest_of_Arguments_Helper_(_pQty_, ...)								\
	LUL_Rest_of_Arguments_Helper_2_ (_pQty_, __VA_ARGS__)

#define LUL_Rest_of_Arguments_Helper_2_(_pQty_, ...)							\
	LUL_Rest_of_Arguments_Helper_##_pQty_(__VA_ARGS__)

#define LUL_Rest_of_Arguments_Helper_One_(_pFirst_)
#define LUL_Rest_of_Arguments_Helper_Two_or_More_(_pFirst_, ...)				\
	, __VA_ARGS__

#define LUL_Number_of_Arguments_(...) \
    LUL_Select_10th_Argument_ (__VA_ARGS__, Two_or_More_, Two_or_More_, 		\
    	Two_or_More_, Two_or_More_, Two_or_More_, Two_or_More_, Two_or_More_,	\
    		Two_or_More_, One_, _pIgnoreMe_)

#define LUL_Select_10th_Argument_(_pArg1_, _pArg2_, _pArg3_, _pArg4_, _pArg5_,	\
	_pArg6_, _pArg7_, _pArg8_, _pArg9_, _pArg10_, ...)							\
		_pArg10_


//	Error Macros
//	Opt_Post_ macros will report Status if _pV_ is true, in the course of
//	carrying out their other actions. Post_ macros will always report Status.
//	All other macros do not report Status, simply passing it along if that is
//	one of the functions of the macro. Note that though some of these macros
//	are many lines long, they’ll mostly compile down to a few lines of actual
//	code at most.
//	SEEME Arguments for the following macros are defined thusly:
//		_pV_	verbosity; true to post new status items, false to defer
//		_pS_	status; Status object; may be rvalue, lvalue, etc.
//		_pA_	action; (optional) action to take (if test is positive)
//		_pNS_	new status; Status object to be chained to _pS_; may be
//					rvalue, lvalue, etc.
//		_pSE_	StatusException; existing object to be acted upon; may be
//					rvalue, lvalue, etc.
//		_pG_	group; component for in-line-generated Status object
//		_pID_	ID; component for in-line-generated Status object
//		...		var args component for in-line-generated Status object
//		_pT_	Status test; Status member function test to be performed
//		_pP_	pointer; pointer to be compared to nullptr
//		_pTest_	test; statement to be evaluated; must resolve to true or false
//	SEEME A previous iteration of these macros attempted to differentiate
//	between rvalue, const lvalue, by-value, etc. arguments in an attempt to
//	minimize copies; this got silly-complex and never worked quite right. It
//	should work with helper templates and perfect forwarding, but we’re well
//	over the effort-to-gain threshold at this time.
//	APIME These are macros instead of inline functions since functions must
//	return: some of these macros should cause the caller to return a Status
//	value immediately, but are not allowed to use Exceptions for thst purpose.

//	generic Status macros
#define LUL_Opt_Post_Status_(_pV_, _pS_)										\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);						\
	} while (false)

#define LUL_Opt_Post_Status_Return_(_pV_, _pS_)									\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);						\
		return _pS_;															\
	} while (false)

#define LUL_Opt_Post_Status_Throw_(_pV_, _pS_)									\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);						\
		throw LUL_v_::StatusException {_pS_};									\
	} while (false)

#define LUL_Opt_Post_Status_Rethrow_(_pV_, _pSE_)								\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pSE_.Get_Status());		\
		throw _pSE_;															\
	} while (false)

#define LUL_Opt_Post_Chain_Status_(_pV_, _pS_, _pNS_)							\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);					\
		(void) _pS_.Chain (_pNS_);												\
	} while (false)

#define LUL_Opt_Post_Chain_Status_Return_(_pV_, _pS_, _pNS_)					\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);					\
		(void) _pS_.Chain (_pNS_);												\
		return _pS_;															\
	} while (false)

#define LUL_Opt_Post_Chain_Status_Throw_(_pV_, _pS_, _pNS_)						\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);					\
		(void) _pS_.Chain (_pNS_);												\
		throw LUL_v_::StatusException {_pS_};									\
	} while (false)

#define LUL_Opt_Post_Chain_Status_Rethrow_(_pV_, _pSE_, _pNS_)					\
	do {																		\
		if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);					\
		(void) _pSE_.Get_Status().Chain (_pNS_);								\
		throw _pSE_;															\
	} while (false)


//	explicit Status macros
#define LUL_Post_Status_(_pS_)													\
	LUL_Opt_Post_Status_ (true, _pS_)

#define LUL_Post_Status_Return_(_pS_)											\
	LUL_Opt_Post_Status_Return_ (true, _pS_)

#define LUL_Status_Return_(_pS_)												\
	LUL_Opt_Post_Status_Return_ (false, _pS_)

#define LUL_Post_Status_Throw_(_pS_)											\
	LUL_Opt_Post_Status_Throw_ (true, _pS_)

#define LUL_Post_Status_Rethrow_(_pSE_)											\
	LUL_Opt_Post_Status_Rethrow_ (true, _pSE_)

#define LUL_Post_Chain_Status_(_pS_, _pNS_)										\
	LUL_Opt_Post_Chain_Status_ (true, _pS_, _pNS_)

#define LUL_Chain_Status_(_pS_, _pNS_)											\
	LUL_Opt_Post_Chain_Status_ (false, _pS_, _pNS_)

#define LUL_Post_Chain_Status_Return_(_pS_, _pNS_)								\
	LUL_Opt_Post_Chain_Status_Return_ (true, _pS_, _pNS_)

#define LUL_Chain_Status_Return_(_pS_, _pNS_)									\
	LUL_Opt_Post_Chain_Status_Return_ (false, _pS_, _pNS_)

#define LUL_Post_Chain_Status_Throw_(_pS_, _pNS_)								\
	LUL_Opt_Post_Chain_Status_Throw_ (true, _pS_, _pNS_)

#define LUL_Post_Chain_Status_Rethrow_(_pSE_, _pNS_)							\
	LUL_Opt_Post_Chain_Status_Rethrow_ (true, _pSE_, _pNS_)

#define LUL_Chain_Status_Throw_(_pS_, _pNS_)									\
	LUL_Opt_Post_Chain_Status_Throw_ (false, _pS_, _pNS_)

#define LUL_Chain_Status_Rethrow_(_pSE_, _pNS_)									\
	LUL_Opt_Post_Chain_Status_Rethrow_ (false, _pSE_, _pNS_)


//	generic Error macros; these create a Status object from the inputs
#define LUL_Opt_Post_Error_(_pV_, _pG_, _pID_, ...)								\
	LUL_Opt_Post_Status_ (_pV_, 												\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Error_Return_(_pV_, _pG_, _pID_, ...)						\
	LUL_Opt_Post_Status_Return_ (_pV_,											\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Error_Throw_(_pV_, _pG_, _pID_, ...)						\
	LUL_Opt_Post_Status_Throw_ (_pV_,											\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Chain_Error_(_pV_, _pS_, _pG_, _pID_, ...)					\
	LUL_Opt_Post_Chain_Status_ (_pV_, _pS_,										\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Chain_Error_Return_(_pV_, _pS_, _pG_, _pID_, ...)			\
	LUL_Opt_Post_Chain_Status_Return_ (_pV_, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Chain_Error_Throw_(_pV_, _pS_, _pG_, _pID_, ...)			\
	LUL_Opt_Post_Chain_Status_Throw_ (_pV_, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));

#define LUL_Opt_Post_Chain_Error_Rethrow_(_pV_, _pSE_, _pG_, _pID_, ...)		\
	LUL_Opt_Post_Chain_Status_Rethrow_ (_pV_, _pSE_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__));


//	explicit Error macros; these create a Status object from the inputs
#define LUL_Post_Error_(_pG_, _pID_, ...)										\
	LUL_Opt_Post_Error_ (true, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Error_Return_(_pG_, _pID_, ...)								\
	LUL_Opt_Post_Error_Return_ (true, _pG_, _pID_, __VA_ARGS__)

#define LUL_Error_Return_(_pG_, _pID_, ...)										\
	LUL_Opt_Post_Error_Return_ (false, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Error_Throw_(_pG_, _pID_, ...)									\
	LUL_Opt_Post_Error_Throw_ (true, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Chain_Error_(_pS_, _pG_, _pID_, ...)							\
	LUL_Opt_Post_Chain_Error_ (true, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Chain_Error_(_pS_, _pG_, _pID_, ...)								\
	LUL_Opt_Post_Chain_Error_ (false, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Chain_Error_Return_(_pS_, _pG_, _pID_, ...)					\
	LUL_Opt_Post_Chain_Error_Return_ (true, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Chain_Error_Return_(_pS_, _pG_, _pID_, ...)							\
	LUL_Opt_Post_Chain_Error_Return_ (false, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Chain_Error_Throw_(_pS_, _pG_, _pID_, ...)						\
	LUL_Opt_Post_Chain_Error_Throw_ (true, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Post_Chain_Error_Rethrow_(_pSE_, _pG_, _pID_, ...)					\
	LUL_Opt_Post_Chain_Error_Rethrow_ (true, _pSE_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Chain_Error_Throw_(_pS_, _pG_, _pID_, ...)							\
	LUL_Opt_Post_Chain_Error_Throw_ (false, _pS_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Chain_Error_Rethrow_(_pSE_, _pG_, _pID_, ...)						\
	LUL_Opt_Post_Chain_Error_Rethrow_ (false, _pSE_, _pG_, _pID_, __VA_ARGS__)


//	generic Status test macros
#define LUL_Test_Opt_Post_Status_(_pV_, _pT_, _pS_)								\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);					\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Status_Return_(_pV_, _pT_, _pS_)						\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);					\
			return _pS_;														\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Status_Throw_(_pV_, _pT_, _pS_)						\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pS_);					\
			throw LUL_v_::StatusException {_pS_};								\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Status_Rethrow_(_pV_, _pT_, _pSE_)					\
	do {																		\
		if (_pSE_.Get_Status()._pT_()) {										\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pSE_.Get_Status());	\
			throw _pSE_;														\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Chain_Status_(_pV_, _pT_, _pS_, _pNS_)				\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);				\
			(void) _pS_.Chain (_pNS_);											\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Chain_Status_Return_(_pV_, _pT_, _pS_, _pNS_)			\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);				\
			(void) _pS_.Chain (_pNS_);											\
			return _pS_;														\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Chain_Status_Throw_(_pV_, _pT_, _pS_, _pNS_)			\
	do {																		\
		if (_pS_._pT_()) {														\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);				\
			(void) _pS_.Chain (_pNS_);											\
		}																		\
	} while (false)

#define LUL_Test_Opt_Post_Chain_Status_Rethrow_(_pV_, _pT_, _pSE_, _pNS_)		\
	do {																		\
		if (_pSE_.Get_Status()._pT_()) {										\
			if (_pV_) LUL_v_::ErrorHandler::Get()->Post (_pNS_);				\
			(void) _pSE_.Get_Status().Chain (_pNS_);							\
		}																		\
	} while (false)


//	explicit Status test macros
#define LUL_Test_Post_Status_(_pT_, _pS_)										\
	LUL_Test_Opt_Post_Status_ (true, _pT_, _pS_)

#define LUL_Test_Post_Status_Return_(_pT_, _pS_)								\
	LUL_Test_Opt_Post_Status_Return_ (true, _pT_, _pS_)

#define LUL_Test_Status_Return_(_pT_, _pS_)										\
	LUL_Test_Opt_Post_Status_Return_ (false, _pT_, _pS_)

#define LUL_Test_Status_Throw_(_pT_, _pS_)										\
	LUL_Test_Opt_Post_Status_Throw_ (false, _pT_, _pS_)

#define LUL_Test_Status_Rethrow_(_pT_, _pSE_)									\
	LUL_Test_Opt_Post_Status_Rethrow_ (false, _pT_, _pSE_)

#define LUL_Test_Post_Chain_Status_(_pT_, _pS_, _pNS_)							\
	LUL_Test_Opt_Post_Chain_Status_ (true, _pT_, _pS_, _pNS_)

#define LUL_Test_Chain_Status_(_pT_, _pS_, _pNS_)								\
	LUL_Test_Opt_Post_Chain_Status_ (false, _pT_, _pS_, _pNS_)

#define LUL_Test_Post_Chain_Status_Return_(_pT_, _pS_, _pNS_)					\
	LUL_Test_Opt_Post_Chain_Status_Return_ (true, _pT_, _pS_, _pNS_)

#define LUL_Test_Chain_Status_Return_(_pT_, _pS_, _pNS_)						\
	LUL_Test_Opt_Post_Chain_Status_Return_ (false, _pT_, _pS_, _pNS_)

#define LUL_Test_Post_Chain_Status_Throw_(_pT_, _pS_, _pNS_)					\
	LUL_Test_Opt_Post_Chain_Status_Throw_ (true, _pT_, _pS_, _pNS_)

#define LUL_Test_Post_Chain_Status_Rethrow_(_pT_, _pSE_, _pNS_)					\
	LUL_Test_Opt_Post_Chain_Status_Rethrow_ (true, _pT_, _pSE_, _pNS_)

#define LUL_Test_Chain_Status_Throw_(_pT_, _pS_, _pNS_)							\
	LUL_Test_Opt_Post_Chain_Status_Throw_ (false, _pT_, _pS_, _pNS_)

#define LUL_Test_Chain_Status_Rethrow_(_pT_, _pSE_, _pNS_)						\
	LUL_Test_Opt_Post_Chain_Status_Rethrow_ (false, _pT_, _pSE_, _pNS_)


//	explicit Error test macros
#define LUL_Test_Error_Post_(_pS_)												\
	LUL_Test_Post_Status_ (Error, _pS_)

#define LUL_Test_Error_Post_Return_(_pS_)										\
	LUL_Test_Post_Status_Return_ (Error, _pS_)

#define LUL_Test_Error_Return_(_pS_)											\
	LUL_Test_Status_Return_ (Error, _pS_)

#define LUL_Test_Error_Post_Throw_(_pS_)										\
	LUL_Test_Post_Status_Throw_ (Error, _pS_)

#define LUL_Test_Error_Post_Rethrow_(_pSE_)										\
	LUL_Test_Post_Status_Rethrow_ (Error, _pSE_)

#define LUL_Test_Error_Throw_(_pS_)												\
	LUL_Test_Status_Throw_ (Error, _pS_)

#define LUL_Test_Error_Rethrow_(_pSE_)											\
	LUL_Test_Status_Rethrow_ (Error, _pSE_)

#define LUL_Test_Error_Post_Chain_(_pS_, _pG_, _pID_, ...)						\
	LUL_Test_Post_Chain_Status_ (Error, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Post_Chain_Status_(_pS_, _pNS_)							\
	LUL_Test_Post_Chain_Status_ (Error, _pS_, _pNS_)

#define LUL_Test_Error_Chain_(_pS_, _pG_, _pID_, ...)							\
	LUL_Test_Chain_Status_ (Error, _pS_,										\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Chain_Status_(_pS_, _pNS_)								\
	LUL_Test_Chain_Status_ (Error, _pS_, _pNS_)

#define LUL_Test_Error_Post_Chain_Return_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Return_ (Error, _pS_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Post_Chain_Status_Return_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Return_ (Error, _pS_, _pNS_))

#define LUL_Test_Error_Chain_Return_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Return_ (Error, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Chain_Status_Return_(_pS_, _pNS_)						\
	LUL_Test_Chain_Status_Return_ (Error, _pS_, _pNS_)

#define LUL_Test_Error_Post_Chain_Throw_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Throw_ (Error, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Post_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Rethrow_ (Error, _pSE_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Post_Chain_Status_Throw_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Throw_ (Error, _pS_, _pNS_)

#define LUL_Test_Error_Post_Chain_Status_Rethrow_(_pSE_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Rethrow_ (Error, _pSE_, _pNS_)

#define LUL_Test_Error_Chain_Throw_(_pS_, _pG_, _pID_, ...)						\
	LUL_Test_Chain_Status_Throw_ (Error, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Rethrow_ (Error, _pSE_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Error_Chain_Status_Throw_(_pS_, _pNS_)							\
	LUL_Test_Chain_Status_Throw_ (Error, _pS_, _pNS_)

#define LUL_Test_Error_Chain_Status_Rethrow_(_pSE_, _pNS_)						\
	LUL_Test_Chain_Status_Rethrow_ (Error, _pSE_, _pNS_)


//	explicit Failed test macros
#define LUL_Test_Failed_Post_(_pS_)												\
	LUL_Test_Post_Status_ (Failed, _pS_)

#define LUL_Test_Failed_Post_Return_(_pS_)										\
	LUL_Test_Post_Status_Return_ (Failed, _pS_)

#define LUL_Test_Failed_Return_(_pS_)											\
	LUL_Test_Status_Return_ (Failed, _pS_)

#define LUL_Test_Failed_Post_Throw_(_pS_)										\
	LUL_Test_Post_Status_Throw_ (Failed, _pS_)

#define LUL_Test_Failed_Post_Rethrow_(_pSE_)									\
	LUL_Test_Post_Status_Rethrow_ (Failed, _pSE_)

#define LUL_Test_Failed_Throw_(_pS_)											\
	LUL_Test_Status_Throw_ (Failed, _pS_)

#define LUL_Test_Failed_Rethrow_(_pSE_)											\
	LUL_Test_Status_Rethrow_ (Failed, _pSE_)

#define LUL_Test_Failed_Post_Chain_(_pS_, _pG_, _pID_, ...)						\
	LUL_Test_Post_Chain_Status_ (Failed, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Post_Chain_Status_(_pS_, _pNS_)							\
	LUL_Test_Post_Chain_Status_ (Failed, _pS_, _pNS_)

#define LUL_Test_Failed_Chain_(_pS_, _pG_, _pID_, ...)							\
	LUL_Test_Chain_Status_ (Failed, _pS_,										\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Chain_Status_(_pS_, _pNS_)								\
	LUL_Test_Chain_Status_ (Failed, _pS_, _pNS_)

#define LUL_Test_Failed_Post_Chain_Return_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Return_ (Failed, _pS_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Post_Chain_Status_Return_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Return_ (Failed, _pS_, _pNS_))

#define LUL_Test_Failed_Chain_Return_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Return_ (Failed, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Chain_Status_Return_(_pS_, _pNS_)						\
	LUL_Test_Chain_Status_Return_ (Failed, _pS_, _pNS_)

#define LUL_Test_Failed_Post_Chain_Throw_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Throw_ (Failed, _pS_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Post_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)			\
	LUL_Test_Post_Chain_Status_Rethrow_ (Failed, _pSE_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Post_Chain_Status_Throw_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Throw_ (Failed, _pS_, _pNS_)

#define LUL_Test_Failed_Post_Chain_Status_Rethrow_(_pSE_, _pNS_)				\
	LUL_Test_Post_Chain_Status_Rethrow_ (Failed, _pSE_, _pNS_)

#define LUL_Test_Failed_Chain_Throw_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Throw_ (Failed, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Rethrow_ (Failed, _pSE_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Failed_Chain_Status_Throw_(_pS_, _pNS_)						\
	LUL_Test_Chain_Status_Throw_ (Failed, _pS_, _pNS_)

#define LUL_Test_Failed_Chain_Status_Rethrow_(_pSE_, _pNS_)						\
	LUL_Test_Chain_Status_Rethrow_ (Failed, _pSE_, _pNS_)


//	explicit Warning test macros
#define LUL_Test_Warning_Post_(_pS_)											\
	LUL_Test_Post_Status_ (Warning, _pS_)

#define LUL_Test_Warning_Post_Return_(_pS_)										\
	LUL_Test_Post_Status_Return_ (Warning, _pS_)

#define LUL_Test_Warning_Return_(_pS_)											\
	LUL_Test_Status_Return_ (Warning, _pS_)

#define LUL_Test_Warning_Post_Throw_(_pS_)										\
	LUL_Test_Post_Status_Throw_ (Warning, _pS_)

#define LUL_Test_Warning_Post_Rethrow_(_pSE_)									\
	LUL_Test_Post_Status_Rethrow_ (Warning, _pSE_)

#define LUL_Test_Warning_Throw_(_pS_)											\
	LUL_Test_Status_Throw_ (Warning, _pS_)

#define LUL_Test_Warning_Rethrow_(_pSE_)										\
	LUL_Test_Status_Rethrow_ (Warning, _pSE_)

#define LUL_Test_Warning_Post_Chain_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Post_Chain_Status_ (Warning, _pS_,									\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Post_Chain_Status_(_pS_, _pNS_)						\
	LUL_Test_Post_Chain_Status_ (Warning, _pS_, _pNS_)

#define LUL_Test_Warning_Chain_(_pS_, _pG_, _pID_, ...)							\
	LUL_Test_Chain_Status_ (Warning, _pS_,										\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Chain_Status_(_pS_, _pNS_)								\
	LUL_Test_Chain_Status_ (Warning, _pS_, _pNS_)

#define LUL_Test_Warning_Post_Chain_Return_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Return_ (Warning, _pS_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Post_Chain_Status_Return_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Return_ (Warning, _pS_, _pNS_))

#define LUL_Test_Warning_Chain_Return_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Return_ (Warning, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Chain_Status_Return_(_pS_, _pNS_)						\
	LUL_Test_Chain_Status_Return_ (Warning, _pS_, _pNS_)

#define LUL_Test_Warning_Post_Chain_Throw_(_pS_, _pG_, _pID_, ...)				\
	LUL_Test_Post_Chain_Status_Throw_ (Warning, _pS_,							\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Post_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)			\
	LUL_Test_Post_Chain_Status_Rethrow_ (Warning, _pSE_,						\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Post_Chain_Status_Throw_(_pS_, _pNS_)					\
	LUL_Test_Post_Chain_Status_Throw_ (Warning, _pS_, _pNS_)

#define LUL_Test_Warning_Post_Chain_Status_Rethrow_(_pSE_, _pNS_)				\
	LUL_Test_Post_Chain_Status_Rethrow_ (Warning, _pSE_, _pNS_)

#define LUL_Test_Warning_Chain_Throw_(_pS_, _pG_, _pID_, ...)					\
	LUL_Test_Chain_Status_Throw_ (Warning, _pS_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Chain_Rethrow_(_pSE_, _pG_, _pID_, ...)				\
	LUL_Test_Chain_Status_Rethrow_ (Warning, _pSE_,								\
		LUL_Make_Status_ (_pG_, _pID_, __VA_ARGS__))

#define LUL_Test_Warning_Chain_Status_Throw_(_pS_, _pNS_)						\
	LUL_Test_Chain_Status_Throw_ (Warning, _pS_, _pNS_)

#define LUL_Test_Warning_Chain_Status_Rethrow_(_pSE_, _pNS_)					\
	LUL_Test_Chain_Status_Rethrow_ (Warning, _pSE_, _pNS_)


//	generic Boolean Macros
#define LUL_Test_Nil_Opt_Post_(_pV_, _pP_, _pG_, _pID_, ...)					\
	do {																		\
		if (nullptr == (_pP_))													\
			LUL_Opt_Post_Error_ (_pV_, _pG_, _pID_, __VA_ARGS__)				\
	} while (false)

#define LUL_Test_Nil_Opt_Post_Return_(_pV_, _pP_, _pG_, _pID_, ...)				\
	do {																		\
		if (nullptr == (_pP_))													\
			LUL_Opt_Post_Error_Return_ (_pV_, _pG_, _pID_, __VA_ARGS__)			\
	} while (false)

#define LUL_Test_Nil_Opt_Post_Throw_(_pV_, _pP_, _pG_, _pID_, ...)				\
	do {																		\
		if (nullptr == (_pP_))													\
			LUL_Opt_Post_Error_Throw_ (_pV_, _pG_, _pID_, __VA_ARGS__)			\
	} while (false)

#define LUL_Test_False_Opt_Post_(_pV_, _pT_, _pG_, _pID_, ...)					\
	do {																		\
		if (!(_pT_)) LUL_Opt_Post_Error_ (_pV_, _pG_, _pID_, __VA_ARGS__)		\
	} while (false)

#define LUL_Test_False_Opt_Post_Return_(_pV_, _pT_, _pG_, _pID_, ...)			\
	do {																		\
		if (!(_pT_)) LUL_Opt_Post_Error_Return_ (_pV_, _pG_, _pID_, __VA_ARGS__)\
	} while (false)

#define LUL_Test_False_Opt_Post_Throw_(_pV_, _pT_, _pG_, _pID_, ...)			\
	do {																		\
		if (!(_pT_)) LUL_Opt_Post_Error_Throw_ (_pV_, _pG_, _pID_, __VA_ARGS__)	\
	} while (false)

#define LUL_Test_True_Opt_Post_(_pV_, _pT_, _pG_, _pID_, ...)					\
	do {																		\
		if (!!(_pT_)) LUL_Opt_Post_Error_ (_pV_, _pG_, _pID_, __VA_ARGS__)		\
	} while (false)

#define LUL_Test_True_Opt_Post_Return_(_pV_, _pT_, _pG_, _pID_, ...)			\
	do {																		\
		if (!!(_pT_)) LUL_Opt_Post_Error_Return_ (_pV_, _pG_, _pID_, __VA_ARGS__)\
	} while (false)

#define LUL_Test_True_Opt_Post_Throw_(_pV_, _pT_, _pG_, _pID_, ...)				\
	do {																		\
		if (!!(_pT_)) LUL_Opt_Post_Error_Throw_ (_pV_, _pG_, _pID_, __VA_ARGS__)\
	} while (false)


//	explicit Boolean Macros
#define LUL_Test_Nil_Post_(_pP_, _pG_, _pID_, ...)								\
	LUL_Test_Nil_Opt_Post_ (true, _pP_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_Nil_Post_Return_(_pP_, _pG_, _pID_, ...)						\
	LUL_Test_Nil_Opt_Post_Return_ (true, _pP_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_Nil_Return_(_pP_, _pG_, _pID_, ...)							\
	LUL_Test_Nil_Opt_Post_Return_ (false, _pP_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_Nil_Post_Throw_(_pP_, _pG_, _pID_, ...)						\
	LUL_Test_Nil_Opt_Post_Throw_ (true, _pP_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_Nil_Throw_(_pP_, _pG_, _pID_, ...)								\
	LUL_Test_Nil_Opt_Post_Throw_ (false, _pP_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_False_Post_(_pT_, _pG_, _pID_, ...)							\
	LUL_Test_False_Opt_Post_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_False_Post_Return_(_pT_, _pG_, _pID_, ...)						\
	LUL_Test_False_Opt_Post_Return_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_False_Return_(_pT_, _pG_, _pID_, ...)							\
	LUL_Test_False_Opt_Post_Return_ (false, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_False_Post_Throw_(_pT_, _pG_, _pID_, ...)						\
	LUL_Test_False_Opt_Post_Throw_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_False_Throw_(_pT_, _pG_, _pID_, ...)							\
	LUL_Test_False_Opt_Post_Throw_ (false, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_True_Post_(_pT_, _pG_, _pID_, ...)								\
	LUL_Test_True_Opt_Post_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_True_Post_Return_(_pT_, _pG_, _pID_, ...)						\
	LUL_Test_True_Opt_Post_Return_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_True_Return_(_pT_, _pG_, _pID_, ...)							\
	LUL_Test_True_Opt_Post_Return_ (false, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_True_Post_Throw_(_pT_, _pG_, _pID_, ...)						\
	LUL_Test_True_Opt_Post_Throw_ (true, _pT_, _pG_, _pID_, __VA_ARGS__)

#define LUL_Test_True_Throw_(_pT_, _pG_, _pID_, ...)							\
	LUL_Test_True_Opt_Post_Throw_ (false, _pT_, _pG_, _pID_, __VA_ARGS__)


//	Signal Macros
//	Signals/Asserts are used to test for errors that should NEVER happen and
//	indicate a programmer error. The tests they perform may be expensive or
//	impractical for shipping code. Note that Signals always imply termination,
//	insofar as the program has failed catastrophically. These are not user-
//	facing error tests, and get compiled out in non-debug builds under the
//	assumptions that the underlying problems, if any, have been resolved, and
//	that leaving the tests in will just hurt performance.
#if LUL_CONFIG_debug
	#define LUL_Signal_(...)													\
		do {																	\
			LUL_v_::Status _status_ = LUL_Make_Status_ (						\
				LUL_v_::STRING_GROUP_messages, LUL_v_::STATUS_CODE_signal,		\
					__VA_ARGS__);												\
			LUL_v_::ErrorHandler::Get()->Post (_status_);						\
			std::exit (EXIT_FAILURE);											\
		} while (false)

	#define LUL_Test_True_Signal_(_pTestable_)									\
		do {																	\
			if (!!(_pTestable_)) LUL_Signal_ (#_pTestable_);					\
		} while (false)

	#define LUL_Test_False_Signal_(_pTestable_)									\
		do {																	\
			if (!(_pTestable_)) LUL_Signal_ (#_pTestable_);						\
		} while (false)

	//	LUL_Assert_ is just a convenience alias.
	#define LUL_Assert_(_pTestable_)											\
		LUL_Test_False_Signal_ (_pTestable_)
#else
	#define LUL_Signal_(...)
	#define LUL_Test_True_Signal_(_pTestable_)
	#define LUL_Test_False_Signal_(_pTestable_)
	#define LUL_Assert_(_pTestable_)
#endif	//	LUL_CONFIG_debug


//	basic Exception Macros
#define LUL_Throw_(_pException_, ...)											\
	throw (_pException_ (LUL_v_::VA_To_String (__VA_ARGS__)))

#define LUL_Throw_If_(_pTestable_, _pException_, ...)							\
	do {																		\
		if (!!(_pTestable_)) LUL_Throw_ (_pException_, __VA_ARGS__);			\
	} while (false)


#define LUL_Throw_If_Not_(_pTestable_, _pException_, ...)						\
	do {																		\
		if (!(_pTestable_)) LUL_Throw_ (_pException_, __VA_ARGS__);				\
	} while (false)


#define LUL_Throw_If_Nil_(_pTestable_, _pException_, ...)						\
	do {																		\
		if (nullptr == (_pTestable_))											\
			LUL_Throw_ (_pException_, __VA_ARGS__);								\
	} while (false)
