/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“details/HelperLocale.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a subset of the libc++ 8.0 <__locale> implementation used under the
	University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	The notable differences from the libc++ version include:

	* namespace changes
	* removal of some unneeded preprocessor tests and attributes
	* renaming of various macros to use LUL versions
	* renaming of system-reserved symbols

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif

//	std
#include <locale>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy { namespace details {

[[noreturn]] LUL_VIS_FUNC void __throw_runtime_error(const char*);

template <size_t _Np>
struct __narrow_to_utf8
{
    template <class _OutputIterator, class _CharT>
    _OutputIterator
    operator()(_OutputIterator __s, const _CharT* __wb, const _CharT* __we) const;
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __narrow_to_utf8<8>
{
    template <class _OutputIterator, class _CharT>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const _CharT* __wb, const _CharT* __we) const
    {
        for (; __wb < __we; ++__wb, ++__s)
            *__s = *__wb;
        return __s;
    }
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __narrow_to_utf8<16>
    : public std::codecvt<char16_t, char, std::mbstate_t>
{
    LUL_VIS_INLINE_FUNC
    __narrow_to_utf8() : std::codecvt<char16_t, char, std::mbstate_t>(1) {}

    LUL_VIS_FUNC
    ~__narrow_to_utf8();

    template <class _OutputIterator, class _CharT>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const _CharT* __wb, const _CharT* __we) const
    {
        result __r = ok;
        std::mbstate_t __mb;
        while (__wb < __we && __r != error)
        {
            const int __sz = 32;
            char __buf[__sz];
            char* __bn;
            const char16_t* __wn = (const char16_t*)__wb;
            __r = do_out(__mb, (const char16_t*)__wb, (const char16_t*)__we, __wn,
                         __buf, __buf+__sz, __bn);
            if (__r == codecvt_base::error || __wn == (const char16_t*)__wb)
                __throw_runtime_error("locale not supported");
            for (const char* __p = __buf; __p < __bn; ++__p, ++__s)
                *__s = *__p;
            __wb = (const _CharT*)__wn;
        }
        return __s;
    }
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __narrow_to_utf8<32>
    : public std::codecvt<char32_t, char, std::mbstate_t>
{
    LUL_VIS_INLINE_FUNC
    __narrow_to_utf8() : std::codecvt<char32_t, char, std::mbstate_t>(1) {}

    LUL_VIS_FUNC
    ~__narrow_to_utf8();

    template <class _OutputIterator, class _CharT>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const _CharT* __wb, const _CharT* __we) const
    {
        result __r = ok;
        std::mbstate_t __mb;
        while (__wb < __we && __r != error)
        {
            const int __sz = 32;
            char __buf[__sz];
            char* __bn;
            const char32_t* __wn = (const char32_t*)__wb;
            __r = do_out(__mb, (const char32_t*)__wb, (const char32_t*)__we, __wn,
                         __buf, __buf+__sz, __bn);
            if (__r == codecvt_base::error || __wn == (const char32_t*)__wb)
                __throw_runtime_error("locale not supported");
            for (const char* __p = __buf; __p < __bn; ++__p, ++__s)
                *__s = *__p;
            __wb = (const _CharT*)__wn;
        }
        return __s;
    }
};

template <size_t _Np>
struct __widen_from_utf8
{
    template <class _OutputIterator>
    _OutputIterator
    operator()(_OutputIterator __s, const char* __nb, const char* __ne) const;
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __widen_from_utf8<8>
{
    template <class _OutputIterator>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const char* __nb, const char* __ne) const
    {
        for (; __nb < __ne; ++__nb, ++__s)
            *__s = *__nb;
        return __s;
    }
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __widen_from_utf8<16>
    : public std::codecvt<char16_t, char, std::mbstate_t>
{
    LUL_VIS_INLINE_FUNC
    __widen_from_utf8() : std::codecvt<char16_t, char, std::mbstate_t>(1) {}

    LUL_VIS_FUNC
    ~__widen_from_utf8();

    template <class _OutputIterator>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const char* __nb, const char* __ne) const
    {
        result __r = ok;
        std::mbstate_t __mb;
        while (__nb < __ne && __r != error)
        {
            const int __sz = 32;
            char16_t __buf[__sz];
            char16_t* __bn;
            const char* __nn = __nb;
            __r = do_in(__mb, __nb, __ne - __nb > __sz ? __nb+__sz : __ne, __nn,
                        __buf, __buf+__sz, __bn);
            if (__r == codecvt_base::error || __nn == __nb)
                __throw_runtime_error("locale not supported");
            for (const char16_t* __p = __buf; __p < __bn; ++__p, ++__s)
                *__s = (wchar_t)*__p;
            __nb = __nn;
        }
        return __s;
    }
};

template <>
struct LUL_VIS_CLASS_TEMPLATE __widen_from_utf8<32>
    : public std::codecvt<char32_t, char, std::mbstate_t>
{
    LUL_VIS_INLINE_FUNC
    __widen_from_utf8() : std::codecvt<char32_t, char, std::mbstate_t>(1) {}

    LUL_VIS_FUNC
    ~__widen_from_utf8();

    template <class _OutputIterator>
    LUL_VIS_INLINE_FUNC
    _OutputIterator
    operator()(_OutputIterator __s, const char* __nb, const char* __ne) const
    {
        result __r = ok;
        std::mbstate_t __mb;
        while (__nb < __ne && __r != error)
        {
            const int __sz = 32;
            char32_t __buf[__sz];
            char32_t* __bn;
            const char* __nn = __nb;
            __r = do_in(__mb, __nb, __ne - __nb > __sz ? __nb+__sz : __ne, __nn,
                        __buf, __buf+__sz, __bn);
            if (__r == codecvt_base::error || __nn == __nb)
                __throw_runtime_error("locale not supported");
            for (const char32_t* __p = __buf; __p < __bn; ++__p, ++__s)
                *__s = (wchar_t)*__p;
            __nb = __nn;
        }
        return __s;
    }
};

} } // namespace details, namespace stdproxy

LUL_end_v_namespace


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
