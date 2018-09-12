/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“SpanWrapper.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	Include this instead of including <span> or lulSpan.hpp directly. This
	placeholder will select the correct headers based on availability.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <Lucena-Utilities/lulVersionWrapper.hpp>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulCompilerFlags.hpp>


#ifdef LUL_TEMP_SPAN_NAMESPACE
	#error "LUL_TEMP_SPAN_NAMESPACE should not be defined"
#endif

#if !LUL_CONFIG_force_local_span
	#if LUL_LIBCPP2A_SPAN
		#include <span>

		#define LUL_TEMP_SPAN_NAMESPACE LUL_std_abi
	#endif
#endif

#ifdef LUL_TEMP_SPAN_NAMESPACE
	LUL_begin_v_namespace

	namespace stdproxy {

	//	26.7.1:
	using LUL_TEMP_SPAN_NAMESPACE::dynamic_extent;

	template <class ElementType, std::ptrdiff_t Extent = dynamic_extent>
	using span = LUL_TEMP_SPAN_NAMESPACE::span <ElementType, Extent>;

	//	SEEME We don’t perfect-forward these in order to avoid ambiguity due to
	//	symmetry when resolving functions. Note that none of the comparisons is
	//	noexcept.
	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator == (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator == <T,X,U,Y> (lhs, rhs);
	}

	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator != (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator != <T,X,U,Y> (lhs, rhs);
	}

	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator < (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator < <T,X,U,Y> (lhs, rhs);
	}

	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator <= (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator <= <T,X,U,Y> (lhs, rhs);
	}

	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator > (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator > <T,X,U,Y> (lhs, rhs);
	}

	template <class T, ptrdiff_t X, class U, ptrdiff_t Y>
	LUL_VIS_INLINE inline constexpr bool operator >= (
		span <T, X> lhs,
		span <U, Y> rhs)
	{
		return LUL_TEMP_SPAN_NAMESPACE::operator >= <T,X,U,Y> (lhs, rhs);
	}

	template <class ElementType, std::ptrdiff_t Extent>
	auto inline as_bytes (
		span <ElementType, Extent> s) noexcept
	{
		return LUL_TEMP_SPAN_NAMESPACE::as_bytes <ElementType, Extent> (s);
	}

	template <class ElementType, std::ptrdiff_t Extent>
	auto inline as_writable_bytes (
		span <ElementType, Extent> s) noexcept
	{
		return LUL_TEMP_SPAN_NAMESPACE::as_writable_bytes <ElementType, Extent> (s);
	}

	}	//	namespace stdproxy

	LUL_end_v_namespace

	#define LUL_CONFIG_std_span_supported 1

	#undef LUL_TEMP_SPAN_NAMESPACE
#else
	#include <Lucena-Utilities/lulSpan.hpp>

	//	Nothing further needs to be done as our reference implementation is
	//	already in the correct namespace with the expected names.
#endif	//	LUL_TEMP_SPAN_NAMESPACE

#ifndef LUL_CONFIG_std_span_supported
	#define LUL_CONFIG_std_span_supported 0
#endif	//	LUL_CONFIG_std_span_supported
