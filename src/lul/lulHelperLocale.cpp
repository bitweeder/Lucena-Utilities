/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“HelperLocale.cpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked-up copy of portions of locale.cpp taken from
	libc++ 7, and used under the University of Illinois/NCSA Open Source
	License. See “license/libc++ License” for details.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only


//	std
#include <exception>
#include <locale>
#include <string>


//	lul
#include <Lucena-Utilities/lulFeatureSetup.hpp>

#include <Lucena-Utilities/details/lulHelperLocale.hpp>

#include "lulConfig_priv.hpp"


LUL_begin_v_namespace

namespace stdproxy::details {

/*------------------------------------------------------------------------------
*/

// __throw_runtime_error
[[maybe_unused]] [[noreturn]] static void __throw_runtime_error(const std::string &msg)
{
    throw std::runtime_error(msg);
}

// __narrow_to_utf8<16>
__narrow_to_utf8<16>::~__narrow_to_utf8()
{
}

// __narrow_to_utf8<32>
__narrow_to_utf8<32>::~__narrow_to_utf8()
{
}

// __widen_from_utf8<16>
__widen_from_utf8<16>::~__widen_from_utf8()
{
}

// __widen_from_utf8<32>
__widen_from_utf8<32>::~__widen_from_utf8()
{
}


/*----------------------------------------------------------------------------*/

}	//	stdproxy::detail

LUL_end_v_namespace


#endif	//	LUL_CONFIG_headers_only
