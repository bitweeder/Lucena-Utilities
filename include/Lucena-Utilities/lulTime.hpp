/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Time.hpp”
	Copyright © 2011-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <chrono>
#include <limits>
#include <ratio>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


LUL_begin_v_namespace


/*------------------------------------------------------------------------------
	Type Definitions

	Clock type bases are platform-dependent, and not necessarily equivalent. As
	per the Standard, SysClock represents wall clock time, while Clock advances
	with real time.
*/

//	System clock convenience types
using SysClock = LUL_std_abi::chrono::system_clock;
using SysTimePoint = typename SysClock::time_point;
using SysDuration = typename SysClock::duration;

//	Steady clock convenience types; used for timing; hopefully has at the least
//	the resolution of SysClock, but there’s no guarantee.
using Clock = LUL_std_abi::chrono::steady_clock;
using TimePoint = typename Clock::time_point;
using Duration = typename Clock::duration;


/*------------------------------------------------------------------------------
	Time Constants

	The SYS_CLOCK_ prefix is reserved for constants defined in terms of the
	std::chrono system clock in order to differentiate them from other time-
	oriented constants. Similarly, CLOCK_ is reserved for the std::chrono
	steady clock.

	Note that we’re overly conservative in our application of duration_cast,
	but these are all constexprs anyway, so it doesn’t really matter.

	These are effectively aliases of chrono::duration_cast. We can’t simply use
	the chrono convenience types (e.g., chrono::seconds, chrono::nanoseconds,
	etc.) since they may require a narrowing conversion to map to the relevant
	duration type, hence requiring a duration_cast instead of being able to use
	an implicit conversion constructor. From this standpoint, these are more
	generalized convenience types.
*/

constexpr Duration		CLOCK_zero				{Duration::zero()};
constexpr Duration		CLOCK_infinite			{Duration::max()};

constexpr SysDuration	SYS_CLOCK_zero			{SysDuration::zero()};
constexpr SysDuration	SYS_CLOCK_infinite		{SysDuration::max()};


template <class _pRep>
inline constexpr Duration
CLOCK_microseconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::micro> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_milliseconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::milli> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_ticks (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <1, 60>> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_seconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_minutes (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <60>> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_hours (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <3600>> (f));
}

template <class _pRep>
inline constexpr Duration
CLOCK_days (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <Duration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <86400>> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_microseconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::micro> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_milliseconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::milli> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_ticks (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <1, 60>> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_seconds (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_minutes (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <60>> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_hours (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <3600>> (f));
}

template <class _pRep>
inline constexpr SysDuration
SYS_CLOCK_days (const _pRep & f)
{
    return LUL_std_abi::chrono::duration_cast <SysDuration> (LUL_std_abi::chrono::duration <_pRep, LUL_std_abi::ratio <86400>> (f));
}


/*----------------------------------------------------------------------------*/

LUL_end_v_namespace
