/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Iterator.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <iterator>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


LUL_begin_v_namespace

namespace details {
	template <typename T>
	struct reverse_wrapper
	{
		T & iterable;

		LUL_VIS_INLINE_FUNC
		constexpr auto begin() noexcept (
			noexcept (std::rbegin (iterable)))
		{
			return std::rbegin (iterable);
		}

		LUL_VIS_INLINE_FUNC
		constexpr auto begin() const noexcept (
			noexcept (std::crbegin (iterable)))
		{
			return std::crbegin (iterable);
		}

		LUL_VIS_INLINE_FUNC
		constexpr auto cbegin() const noexcept (
			noexcept (std::crbegin (iterable)))
		{
			return std::crbegin (iterable);
		}

		LUL_VIS_INLINE_FUNC
		constexpr auto end() noexcept (
			noexcept (std::rend (iterable)))
		{
			return std::rend (iterable);
		}

		LUL_VIS_INLINE_FUNC
		constexpr auto end() const noexcept (
			noexcept (std::crend (iterable)))
		{
			return std::crend (iterable);
		}

		LUL_VIS_INLINE_FUNC
		constexpr auto cend() const noexcept (
			noexcept (std::crend (iterable)))
		{
			return std::crend (iterable);
		}
	};
}

template <typename T>
LUL_VIS_INLINE_FUNC
details::reverse_wrapper <T>  reverse (T && iterable)
{
	return {std::forward <T> (iterable)};
}

LUL_end_v_namespace
