/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“ObserverPtr.hpp”
	Copyright © 2015-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is an implementation of the C++17-proposed observer_ptr.

	Comments and structure are taken from Library Fundamentals, Version 3
	<http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4758.html>.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <functional>
#include <memory>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy {


/*------------------------------------------------------------------------------
*/

//	5.2, Non-owning pointers
template <class T>
class observer_ptr
{
	private:
		using pointer = std::add_pointer_t <T>;				//	exposition-only
		using reference = std::add_lvalue_reference_t <T>;	//	exposition-only

		static_assert (!std::is_reference_v <T>, "");


	public:
		//	publish our template parameter and variations thereof
		using element_type = T;

		//	5.2.2, observer_ptr constructors
		//	default c’tor
		constexpr observer_ptr() noexcept;

		//	pointer-accepting c’tors
		constexpr observer_ptr (std::nullptr_t) noexcept;
		constexpr explicit observer_ptr (pointer) noexcept;

		//	copying c’tors (in addition to compiler-generated copy c’tor)
		template <class U,
			typename = std::enable_if_t <
				std::is_convertible_v <std::add_pointer_t <U>>>>
				//	requires
				//		std::is_convertible_v <std::add_pointer_t <U>>
		constexpr observer_ptr (observer_ptr <U>) noexcept;

		//	5.2.3, observer_ptr observers
		constexpr pointer get() const noexcept;
		constexpr reference operator*() const;
		constexpr pointer operator->() const noexcept;
		constexpr explicit operator bool() const noexcept;

		//	5.2.4, observer_ptr conversions
		constexpr explicit operator pointer() const noexcept;

		//	5.2.5, observer_ptr modifiers
		constexpr pointer release() noexcept;
		constexpr void reset (pointer = nullptr) noexcept;
		constexpr void swap (observer_ptr &) noexcept;


	private:
		pointer _p;
};


/*------------------------------------------------------------------------------
	observer_ptr implementation
*/

template <class T>
constexpr
observer_ptr <T>::observer_ptr() noexcept
	:	_p {nullptr}
{
}

template <class T>
constexpr
observer_ptr <T>::observer_ptr (
	std::nullptr_t) noexcept
	:	_p {nullptr}
{
}

template <class T>
constexpr
observer_ptr <T>::observer_ptr (
	pointer p) noexcept
	:	_p {p}
{
}

template <class T>
template <class U, typename>
constexpr
observer_ptr <T>::observer_ptr (
	observer_ptr <U> p) noexcept
	:	_p {p.get()}
{
}

template <class T>
constexpr auto
observer_ptr <T>::get() const noexcept -> pointer
{
	return _p;
}

template <class T>
constexpr auto
observer_ptr <T>::operator*() const -> reference
{
	//	C++2a
//	[[assert: get() != nullptr]];

	return *get();
}

template <class T>
constexpr auto
observer_ptr <T>::operator->() const noexcept -> pointer
{
	return get();
}

template <class T>
constexpr
observer_ptr <T>::operator bool() const noexcept
{
	return nullptr != get();
}

template <class T>
constexpr
observer_ptr <T>::operator pointer() const noexcept
{
	return get();
}

template <class T>
constexpr auto
observer_ptr <T>::release() noexcept -> pointer
{
	auto nrv = get();

	reset();
	return nrv;
}

template <class T>
constexpr void
observer_ptr <T>::reset (
	pointer p) noexcept
{
	_p = p;
}

template <class T>
constexpr void
observer_ptr <T>::swap (
	observer_ptr & p) noexcept
{
	std::swap (_p, p._p);
}


/*------------------------------------------------------------------------------
	5.2.6 observer_ptr specialized algorithms
*/

template <class T>
void swap (
	observer_ptr <T> & lhs,
	observer_ptr <T> & rhs) noexcept
{
	lhs.swap (rhs);
}

template <class T>
observer_ptr <T>
make_observer (
	T * p) noexcept
{
	return observer_ptr <T> (p);
}

template <class T, class U>
bool operator == (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return lhs.get() == rhs.get();
}

template <class T, class U>
bool operator != (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return !(lhs == rhs);
}

template <class T>
bool operator == (
	observer_ptr <T> lhs,
	std::nullptr_t) noexcept
{
	return !lhs;
}

template <class T>
bool operator == (
	std::nullptr_t,
	observer_ptr <T> rhs) noexcept
{
	return !rhs;
}

template <class T>
bool operator != (
	observer_ptr <T> lhs,
	std::nullptr_t) noexcept
{
	return (bool) lhs;
}

template <class T>
bool operator != (
	std::nullptr_t,
	observer_ptr <T> rhs) noexcept
{
	return (bool) rhs;
}

template <class T, class U>
bool operator < (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return std::less <std::common_type_t <std::add_pointer_t <T>, std::add_pointer_t <U>>>{} (
		lhs.get(), rhs.get());
}

template <class T, class U>
bool operator > (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return rhs < lhs;
}

template <class T, class U>
bool operator <= (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return !(rhs < lhs);
}

template <class T, class U>
bool operator >= (
	observer_ptr <T> lhs,
	observer_ptr <U> rhs)
{
	return !(lhs < rhs);
}


/*----------------------------------------------------------------------------*/

}	//	stdproxy

LUL_end_v_namespace


/*------------------------------------------------------------------------------
	8.12.7 observer_ptr hash support
*/

//	We inject this directly into std, which is legal for partial
//	specializations of class templates dependent on user-defined types.
namespace std {

//	Note the explicit use of a versioned namespace when referring to optional;
//	this is needed to avoid potential collisions.
template <typename T>
struct hash <LUL_v_::stdproxy::observer_ptr <T>>
{
	using result_type = size_t;
	using argument_type = LUL_v_::stdproxy::observer_ptr <T>;

	size_t operator() (
		const LUL_v_::stdproxy::observer_ptr <T> & p) const noexcept (
			noexcept (hash <add_pointer_t <T>>{} (p.get())))
	{
		return hash <add_pointer_t <T>>{}(p.get());
	}
};

}	//	namespace std
