/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Any.cpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This is a gently hacked up copy of libc++’s any implementation, distributed
	under the MIT Open Source License. See LICENSE.TXT for details. The only
	major differences from the original are a namespace change and the removal
	of some unneeded preprocessor tests and attributes.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>
#include <Lucena-Utilities/lulAny.hpp>


LUL_begin_namespace

namespace stdproxy {


/*------------------------------------------------------------------------------
*/

const char * bad_any_cast::what() const noexcept
{
	return "bad_any_cast";
}


/*----------------------------------------------------------------------------*/

}	//	namespace stdproxy

LUL_end_namespace

#include <experimental/__config>
