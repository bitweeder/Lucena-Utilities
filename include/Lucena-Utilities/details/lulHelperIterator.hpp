/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“details/HelperIterator.hpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a subset of the libc++ 8.0 <iterator> implementation, namely some
	of the private bits, used under the University of Illinois/NCSA Open Source
	License. See “license/libc++ License” for details.

	The <span> and <filesystem> implementations require this.

	The notable differences from the libc++ version include:
		- namespace changes
		- removal of some unneeded preprocessor tests and attributes
		- renaming of various macros to use LUL versions
		- renaming of (some) reserved symbols

	SEEME Remaining (predantic) work needed:
		- rename `__` symbols
		- rename `\b_[A-Z]` symbols

------------------------------------------------------------------------------*/


#pragma once

#if __GNUC__ >= 4
	#pragma GCC system_header
#elif defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif

//	std
#include <iterator>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy { namespace details {

//	__has_iterator_typedefs
template <class _Tp>
struct __has_iterator_typedefs
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(
    	typename std::__void_t<typename _Up::iterator_category>::type* = 0,
		typename std::__void_t<typename _Up::difference_type>::type* = 0,
		typename std::__void_t<typename _Up::value_type>::type* = 0,
		typename std::__void_t<typename _Up::reference>::type* = 0,
		typename std::__void_t<typename _Up::pointer>::type* = 0
	);
public:
    static const bool value = sizeof(__test<_Tp>(0,0,0,0,0)) == 1;
};

//	__has_iterator_category
template <class _Tp>
struct __has_iterator_category
{
private:
    struct __two {char __lx; char __lxx;};
    template <class _Up> static __two __test(...);
    template <class _Up> static char __test(typename _Up::iterator_category* = 0);
public:
    static const bool value = sizeof(__test<_Tp>(0)) == 1;
};

template <class _Tp, class _Up, bool = __has_iterator_category<std::iterator_traits<_Tp> >::value>
struct __has_iterator_category_convertible_to
    : public std::integral_constant<bool, std::is_convertible<typename std::iterator_traits<_Tp>::iterator_category, _Up>::value>
{};

template <class _Tp, class _Up>
struct __has_iterator_category_convertible_to<_Tp, _Up, false> : public std::false_type {};

template <class _Tp>
struct __is_input_iterator : public __has_iterator_category_convertible_to<_Tp, std::input_iterator_tag> {};

template <class _Tp>
struct __is_forward_iterator : public __has_iterator_category_convertible_to<_Tp, std::forward_iterator_tag> {};

template <class _Tp>
struct __is_bidirectional_iterator : public __has_iterator_category_convertible_to<_Tp, std::bidirectional_iterator_tag> {};

template <class _Tp>
struct __is_random_access_iterator : public __has_iterator_category_convertible_to<_Tp, std::random_access_iterator_tag> {};

template <class _Tp>
struct __is_exactly_input_iterator
    : public std::integral_constant<bool,
         __has_iterator_category_convertible_to<_Tp, std::input_iterator_tag>::value &&
        !__has_iterator_category_convertible_to<_Tp, std::forward_iterator_tag>::value> {};


// __wrap_iter
template <class _Iter> class __wrap_iter;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator==(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator<(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator!=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator>(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator>=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
bool
operator<=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

template <class _Iter1, class _Iter2>
LUL_VIS_INLINE_FUNC
auto
operator-(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
-> decltype(__x.base() - __y.base());

template <class _Iter>
LUL_VIS_INLINE_FUNC
__wrap_iter<_Iter>
operator+(typename __wrap_iter<_Iter>::difference_type, __wrap_iter<_Iter>) noexcept;

template <class _Ip, class _Op> _Op LUL_VIS_INLINE_FUNC copy(_Ip, _Ip, _Op);
template <class _B1, class _B2> _B2 LUL_VIS_INLINE_FUNC copy_backward(_B1, _B1, _B2);
template <class _Ip, class _Op> _Op LUL_VIS_INLINE_FUNC move(_Ip, _Ip, _Op);
template <class _B1, class _B2> _B2 LUL_VIS_INLINE_FUNC move_backward(_B1, _B1, _B2);

template <class _Tp>
LUL_VIS_INLINE_FUNC
typename std::enable_if
<
    std::is_trivially_copy_assignable<_Tp>::value,
    _Tp*
>::type
__unwrap_iter(__wrap_iter<_Tp*>);

template <class _Iter>
class __wrap_iter
{
public:
    typedef _Iter                                                      iterator_type;
    typedef typename std::iterator_traits<iterator_type>::iterator_category iterator_category;
    typedef typename std::iterator_traits<iterator_type>::value_type        value_type;
    typedef typename std::iterator_traits<iterator_type>::difference_type   difference_type;
    typedef typename std::iterator_traits<iterator_type>::pointer           pointer;
    typedef typename std::iterator_traits<iterator_type>::reference         reference;
private:
    iterator_type __i;
public:
    LUL_VIS_INLINE_FUNC __wrap_iter() noexcept
                : __i{}
    {
    }
    template <class _Up> LUL_VIS_INLINE_FUNC __wrap_iter(const __wrap_iter<_Up>& __u,
        typename std::enable_if<std::is_convertible<_Up, iterator_type>::value>::type* = 0) noexcept
        : __i(__u.base())
    {
    }
    LUL_VIS_INLINE_FUNC reference operator*() const noexcept
    {
        return *__i;
    }
    LUL_VIS_INLINE_FUNC pointer  operator->() const noexcept
    {
        return (pointer)std::addressof(*__i);
    }
    LUL_VIS_INLINE_FUNC __wrap_iter& operator++() noexcept
    {
        ++__i;
        return *this;
    }
    LUL_VIS_INLINE_FUNC __wrap_iter  operator++(int) noexcept
        {__wrap_iter __tmp(*this); ++(*this); return __tmp;}
    LUL_VIS_INLINE_FUNC __wrap_iter& operator--() noexcept
    {
        --__i;
        return *this;
    }
    LUL_VIS_INLINE_FUNC __wrap_iter  operator--(int) noexcept
        {__wrap_iter __tmp(*this); --(*this); return __tmp;}
    LUL_VIS_INLINE_FUNC __wrap_iter  operator+ (difference_type __n) const noexcept
        {__wrap_iter __w(*this); __w += __n; return __w;}
    LUL_VIS_INLINE_FUNC __wrap_iter& operator+=(difference_type __n) noexcept
    {
        __i += __n;
        return *this;
    }
    LUL_VIS_INLINE_FUNC __wrap_iter  operator- (difference_type __n) const noexcept
        {return *this + (-__n);}
    LUL_VIS_INLINE_FUNC __wrap_iter& operator-=(difference_type __n) noexcept
        {*this += -__n; return *this;}
    LUL_VIS_INLINE_FUNC reference        operator[](difference_type __n) const noexcept
    {
        return __i[__n];
    }

    LUL_VIS_INLINE_FUNC iterator_type base() const noexcept {return __i;}

private:
    LUL_VIS_INLINE_FUNC __wrap_iter(iterator_type __x) noexcept : __i(__x) {}

    template <class _Up> friend class __wrap_iter;
    template <class _CharT, class _Traits, class _Alloc> friend class basic_string;
    template <class _Tp, class _Alloc> friend class _LIBCPP_TEMPLATE_VIS vector;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator==(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator<(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator!=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator>(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator>=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    bool
    operator<=(const __wrap_iter<_Iter1>&, const __wrap_iter<_Iter2>&) noexcept;

    template <class _Iter1, class _Iter2>
    friend
    auto
    operator-(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
    -> decltype(__x.base() - __y.base());

    template <class _Iter1>
    friend
    __wrap_iter<_Iter1>
    operator+(typename __wrap_iter<_Iter1>::difference_type, __wrap_iter<_Iter1>) noexcept;

    template <class _Ip, class _Op> friend _Op copy(_Ip, _Ip, _Op);
    template <class _B1, class _B2> friend _B2 copy_backward(_B1, _B1, _B2);
    template <class _Ip, class _Op> friend _Op move(_Ip, _Ip, _Op);
    template <class _B1, class _B2> friend _B2 move_backward(_B1, _B1, _B2);

    template <class _Tp>
    friend
    typename std::enable_if
    <
        std::is_trivially_copy_assignable_v<_Tp>,
        _Tp*
    >::type
    __unwrap_iter(__wrap_iter<_Tp*>);
};

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator==(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return __x.base() == __y.base();
}

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator<(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return __x.base() < __y.base();
}

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return !(__x == __y);
}

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return __y < __x;
}

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return !(__x < __y);
}

template <class _Iter1, class _Iter2>
inline LUL_VIS_INLINE_FUNC
bool
operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter2>& __y) noexcept
{
    return !(__y < __x);
}

template <class _Iter1>
inline LUL_VIS_INLINE_FUNC
bool
operator!=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) noexcept
{
    return !(__x == __y);
}

template <class _Iter1>
inline LUL_VIS_INLINE_FUNC
bool
operator>(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) noexcept
{
    return __y < __x;
}

template <class _Iter1>
inline LUL_VIS_INLINE_FUNC
bool
operator>=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) noexcept
{
    return !(__x < __y);
}

template <class _Iter1>
inline LUL_VIS_INLINE_FUNC
bool
operator<=(const __wrap_iter<_Iter1>& __x, const __wrap_iter<_Iter1>& __y) noexcept
{
    return !(__y < __x);
}


} } // namespace details, namespace stdproxy

LUL_end_v_namespace


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
