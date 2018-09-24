/*------------------------------------------------------------------------------

	Lucena Utilities Library
	“details/HelperIntrinsics.cpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	FIXME This may actually be unnecessary, as it appears to be the artifact
	of a warning suppression scheme that doesn’t affect us.

------------------------------------------------------------------------------*/


//	lul
#include <Lucena-Utilities/lulConfig.hpp>

#if !LUL_CONFIG_headers_only


//	std
#include <climits>


//	lul
#include <Lucena-Utilities/lulFeatureSetup.hpp>


/*------------------------------------------------------------------------------
	This is taken from the libc++ 7 filesystem implementation, and used under
	the University of Illinois/NCSA Open Source License. See
	“license/libc++ License” for details.

	Comments reproduced verbatim from “filesystem/int128builtins.cpp”:

	This file implements __muloti4, and is stolen from the compiler_rt library.

	FIXME: we steal and re-compile it into filesystem, which uses __int128_t,
	and requires this builtin when sanitized. See llvm.org/PR30643
*/

#if LUL_TYPE_HAS_INT128

extern "C" __int128_t __muloti4(__int128_t a, __int128_t b, int* overflow);

extern "C" //	__attribute__((no_sanitize("undefined")))
__int128_t __muloti4(__int128_t a, __int128_t b, int* overflow) {
  const int N = (int)(sizeof(__int128_t) * CHAR_BIT);
  const __int128_t MIN = (__int128_t)1 << (N - 1);
  const __int128_t MAX = ~MIN;
  *overflow = 0;
  __int128_t result = a * b;
  if (a == MIN) {
    if (b != 0 && b != 1)
      *overflow = 1;
    return result;
  }
  if (b == MIN) {
    if (a != 0 && a != 1)
      *overflow = 1;
    return result;
  }
  __int128_t sa = a >> (N - 1);
  __int128_t abs_a = (a ^ sa) - sa;
  __int128_t sb = b >> (N - 1);
  __int128_t abs_b = (b ^ sb) - sb;
  if (abs_a < 2 || abs_b < 2)
    return result;
  if (sa == sb) {
    if (abs_a > MAX / abs_b)
      *overflow = 1;
  } else {
    if (abs_a > MIN / -abs_b)
      *overflow = 1;
  }
  return result;
}

#endif

#endif	//	LUL_CONFIG_headers_only
