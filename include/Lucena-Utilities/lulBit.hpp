/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“Bit.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	FIXME This is only a fragment of the proposed <bit> header; amusingly, the
	bulk of the header was created to support a number of bit-related classes
	that had not been formally approved, while this piece got tagged in and
	approved fairly quickly. The rest of <bit> has since been voted in, and now
	needs to be added to this reference implemetation.

------------------------------------------------------------------------------*/


#pragma once


//	std
#include <cstring>
#include <type_traits>


//	lul
#include <Lucena-Utilities/lulConfig.hpp>


LUL_begin_v_namespace

namespace stdproxy {

/*------------------------------------------------------------------------------
	This is a gently hacked-up copy of JF Bastien’s proposed std::bit_cast
	implementation, used under the MIT License. See “license/bit_cast License”
	for details.

	The notable differences from the original version include:

	* namespace changes
	* eliminatation of macros
	* minor C++17 updates
	* <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0476r2.html>

	SEEME Once concepts are official and implemented, enable_if_t lines can be
	removed and concept lines updated and uncommented.

	Return an object of type To whose *object representation* is equal to
	the object representation of From. If multiple object representations
	could represent the *value representation* of From, then it is
	unspecified which To value is returned. If no *value representation*
	corresponds to To’s object representation then the returned value is
	unspecified.
*/

template <typename To, typename From,
    typename = std::enable_if_t <sizeof (To) == sizeof (From)>,
    typename = std::enable_if_t <std::is_trivially_copyable_v <To>>,
    typename = std::enable_if_t <std::is_trivially_copyable_v <From>>>
		//	requires
		//		sizeof (To) == sizeof (From) &&
    	//		std::is_trivially_copyable_v <To> &&
    	//		std::is_trivially_copyable_v <From>
LUL_VIS_INLINE inline constexpr To bit_cast (const From& from) noexcept
{
	//	APIME Note that it is illegal in C++17 to simply reinterpret_cast
	//	from a float (or float *) to an int (or int *), even if the sizes are
	//	known to be compatible. Similarly, the hoary old float-int union trick
	//	introduces Undefined Behavior in C++17. Finally, note that the use of
	//	aligned storage is necessary to minimize the impact of the memcpy.
	//	SEEME In order to be constexpr, this implementation requires a
	//	constexpr memcpy, which does not exist as of C++17. If the compiler
	//	doesn’t care that this function can -never- be constexpr, then we can
	//	just go ahead and use this implementation.
	//	SEEME Technically, the best results would be obtained by using compiler
	//	intrinsics (e.g., llvm’s bitcast opcode) instead of memcpy, but in
	//	practice all tested compilers not only optimized out the function call,
	//	but also produced a minimal amount of code this way.
	std::aligned_storage_t <sizeof (To), alignof (To)> storage;

	std::memcpy (&storage, &from, sizeof (To));

	return reinterpret_cast <To &> (storage);
}

} // namespace stdproxy

LUL_end_v_namespace
