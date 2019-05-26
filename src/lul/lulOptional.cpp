/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Optional.cpp”
	Copyright © 2018-2019 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s optional implementation,
	distributed under the MIT Open Source License. See LICENSE.TXT for details.
	The only major differences from the original are a namespace change and the
	removal of some unneeded preprocessor tests and attributes.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only

#include <Lucena-Utilities/details/lulOptional.hpp>


LUL_begin_namespace

namespace stdproxy {


/*------------------------------------------------------------------------------
*/

bad_optional_access::~bad_optional_access() noexcept = default;

const char * bad_optional_access::what() const noexcept
{
	return "bad_optional_access";
}


/*----------------------------------------------------------------------------*/

}	//	namespace stdproxy

LUL_end_namespace

#endif	//	LUL_CONFIG_headers_only
