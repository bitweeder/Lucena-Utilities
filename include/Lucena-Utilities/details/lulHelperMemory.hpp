/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“details/HelperMemory.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a subset of libc++’s <memory> implementation, namely some of the
	private bits, used under the University of Illinois/NCSA Open Source
	License. See “license/libc++ License” for details.

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
#include <memory>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulVisibility.hpp>


LUL_begin_v_namespace

namespace stdproxy { namespace details {

template <class _Alloc>
class __allocator_destructor
{
    typedef std::allocator_traits<_Alloc> __alloc_traits;
public:
    typedef typename __alloc_traits::pointer pointer;
    typedef typename __alloc_traits::size_type size_type;
private:
    _Alloc& __alloc_;
    size_type __s_;
public:
    LUL_VIS_INLINE_FUNC __allocator_destructor(_Alloc& __a, size_type __s) noexcept
        : __alloc_(__a), __s_(__s) {}
    LUL_VIS_INLINE_FUNC
    void operator()(pointer __p) noexcept
        {__alloc_traits::deallocate(__alloc_, __p, __s_);}
};

} } // namespace details, namespace stdproxy

LUL_end_v_namespace


#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif
