/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“TypeTraits.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Type traits are typically named in all lower-case, as per STL convention.

	APIME These implementations mostly follow the examples set by libc++ - even
	if they are sometimes just historical artifacts - unless a demonstrably
	better modern formulation presents itself.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <Lucena-Utilities/lulTypeTraitsWrapper.hpp>
#include <utility>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulTypes.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	is_valid

	This is a traits factory which can be used to trivially define otherwise
	difficult-to-build traits, such as whether a given class contains a given
	member function. The idea and implementation are lifted straight from
	Vandevoorde’s C++ Templates, which in turn lifted the technique from Louis
	Dionne.

	Example usage:

	constexpr auto Is_Default_Constructible =
		is_valid ([](auto x) -> decltype ((void) decltype (Type_From_Value (x))()) { });

	Is_Default_Constructible (type_as_value <int>);
		//	true; ints are default-constructible

	Is_Default_Constructible (type_as_value <int &>);
		//	false; references are not default-constructible

	The preceding formulation can appear at namespace or block scope, though
	it’s a non-standard syntax for type traits. By contrast, the following more
	typical syntax can only be used at namespace scope:

	template <typename T>
	using is_default_constructible =
		decltype (Is_Default_Constructible (std::declval <T>()));

	template <typename T>
	constexpr bool is_default_constructible_v =
		is_default_constructible <T>::value;

	This second formulation requires us to explicitly take the value of the
	trait (or use the associated template variable). Also, note that since the
	second formulation is defined in terms of the first, we still have to
	provide the first formulation even if we’re not using it directly.

	Additional examples:

	//	Does the specified type have a data member named “first”? This same
	//	formulation works for finding member functions.
	//	SEEME This formulation does not help for finding specific signatures of
	//	an overloaded member function; additional testing is needed to see if
	//	that’s a solvable case using this particular traits factory.
	constexpr auto Has_First =
		is_valid ([] (auto && x) -> decltype ((void) &x.first) { });

	//	Does the specified type have a member type named “size_type”? Note that
	//	“size_type” in the below expression is not redundant with
	//	remove_reference_t since we’re actually checking the returned type for
	//	a specific member type, in this case, “size_type”. Note that
	//	remove_reference_t is mandatory in this expression, as otherwise we’ll
	//	always return false due to the reference being passed in by _is_valid.
	//	SEEME The original formulation used decay_t instead of
	//	remove_reference_t, which seems inappropriate in context. remove_cvref
	//	is unnecessary since neither constant nor volatile will affect the
	//	outcome, but decay’s array->pointer effect would give incorrect results
	//	in some circumstances.
	constexpr auto Has_size_type =
		is_valid ([] (auto && x) -> typename std::remove_reference_t <decltype (x)>::size_type { });

	//	Is operator < declared for the provided types? This will check all
	//	non-member functions using ADL to find the supported comparison.
	constexpr auto Has_Less =
		is_valid ([](auto && x, auto && y) -> decltype (x < y) { });
*/

//	helper to check the validity of f (args...) for F f and Args... args
template <typename F, typename... Args,
	typename = decltype (std::declval <F>() (std::declval <Args &&>()...))>
std::true_type _is_valid (void *);
	//	no definition needed; this is only used for evaluating the declaration

//	fallback if helper SFINAE’d out
template <typename F, typename... Args>
std::false_type _is_valid (...);
	//	no definition needed; this is only used for evaluating the declaration

//	lambda that takes a lambda f and returns whether calling f (args) is valid;
//	the return value is a bool since we’re effectively calling operator () on
//	a bool_constant.
inline constexpr auto is_valid = [] (auto f) {
	return [] (auto &&... args) {
		return decltype (_is_valid <decltype (f), decltype (args)&&...> (nullptr)) {};
	};
};

//	helper template to represent a type as a value
template <typename T>
struct TypeValue
{
	using type = T;
};

//	helper to wrap a type as a value
template <typename T>
constexpr auto type_as_value = TypeValue <T>{};

//	helper to unwrap a wrapped type in unevaluated contexts
template <typename T>
T Type_From_Value (TypeValue <T>);
	//	no definition needed


/*------------------------------------------------------------------------------
	wrap

	Use this to pass pointers to function(s|templates) as function arguments
	in order to prevent issues with overloading, particulatly when dealing with
	volatile APIs. See:

		<https://akrzemi1.wordpress.com/2018/07/07/functions-in-std/>
*/

//	SEEME Implementation of a functional-style monadic lift, taken from Andrzej
//	Krzemieński.
#define LUL_Wrap_(_pFunc_)														\
	[] (auto&&... args) noexcept (noexcept (									\
		_pFunc_ (std::forward <decltype(args)> (args)...)))						\
			-> decltype (_pFunc_ (std::forward <decltype (args)> (args)...))	\
	{																			\
		return _pFunc_ (std::forward <decltype (args)> (args)...);				\
	}


//	FIXME Experimental implementation that avoids the preprocessor; this is
//	not semantically equivalent, and may not solve the problem. Needs unit
//	testing.
template <class T>
class wrap
{
	//	FIXME is_invocable was originally called is_callable, but was renamed
	//	during C++17 standardization to free up the symbol for a “more pure”
	//	implementation later. Not all environments have caught up to the
	//	change, yet (<cough>Xcode</cough>), and conversion is not one-to-one,
	//	so we’re currently punting, as writing our own implementation seems a
	//	bit silly. Presumably, this will be handled in Xcode 10, as the change
	//	was made to clang some time ago.
	//	SEEME Usage is incorrect; this is here as a placeholder.
//	static_assert (is_invocable_v <T>);

	public:
		wrap (T func) : _f (func) { }

		template <class... Args>
		auto operator () (Args &&... args) noexcept (noexcept (
			std::declval <T>() (std::forward <decltype (args)> (args)...)))
				-> decltype (std::declval <T>() (std::forward <decltype (args)> (args)...))
		{
			return _f (std::forward <decltype (args)> (args)...);
		}

	private:
		T _f;
};


/*------------------------------------------------------------------------------
	is_same_uncvref

	Convenience wrapper for when we want to check for type matches without
	decaying arrays to pointers, i.e., ignoring const, volatile, and reference
	qualifiers.
*/

template <typename T, typename U>
struct is_same_uncvref
	:	std::is_same <stdproxy::remove_cvref_t <T>, stdproxy::remove_cvref_t <U>>
{
};

template <typename T, typename U>
constexpr bool is_same_uncvref_v = is_same_uncvref <T, U>::value;


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
