/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Variant.cpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s <variant> implementation,
	distributed under the MIT Open Source License. See LICENSE.TXT for details.
	The only major differences from the original are a namespace change and the
	removal of some unneeded preprocessor tests and attributes.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/details/lulVariant.hpp>


LUL_begin_namespace

namespace stdproxy {


/*------------------------------------------------------------------------------
*/

bad_variant_access::~bad_variant_access() noexcept = default;

const char * bad_variant_access::what() const noexcept
{
	return "bad_variant_access";
}


/*----------------------------------------------------------------------------*/

}	//	namespace stdproxy

LUL_end_namespace
