/*------------------------------------------------------------------------------------

	Lucena Utilities Library
	“PCH_std.hpp”
	Copyright © 2006-2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This header is intended for use as a prefix header, or at least a component
	of one, specifically, but not necessarily, for building precompiled
	headers. We make no assumptions about previously included headers here,
	prefix or otherwise.

------------------------------------------------------------------------------------*/


#pragma once


//	SEEME We always guard inclusion of system headers when using MSVS due to
//	their noisiness at high warning levels.
//	SEEME This doesn't actually work with /Wall; it's unclear what the problem 
//	is, but it may have to do with conditions at the point of template
//	instantiation (as opposed to template declaration inside the header).
#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (push, 0)
#endif


//	C
#include <cstddef>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>


//	C++
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <queue>
#include <ratio>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#if defined (_MSC_VER) && defined (_WIN32)
	#pragma warning (pop)
#endif


//	C++14, C++17, C++2a wrappers
//	These are useful additions which may not currently be in the Standard or
//	only exist in proposals or other libraries, which lack implementations in
//	a given Standard Library, or which have poorly behaving implementations
//	that require replacement. The wrappers include the native std headers if
//	they’re available and functional; otherwise, they include a reference
//	version of some kind if one is available.
#include <Lucena-Utilities/lulAnyWrapper.hpp>
#include <Lucena-Utilities/lulBitWrapper.hpp>
#include <Lucena-Utilities/lulFilesystemWrapper.hpp>
#include <Lucena-Utilities/lulMemoryWrapper.hpp>
#include <Lucena-Utilities/lulNetworkingWrapper.hpp>
#include <Lucena-Utilities/lulOptionalWrapper.hpp>
#include <Lucena-Utilities/lulSpanWrapper.hpp>
#include <Lucena-Utilities/lulTypeTraitsWrapper.hpp>
#include <Lucena-Utilities/lulVariantWrapper.hpp>


//	This is actually a wrapper, but it completely subsumes the built in
//	header - if it’s present - and is always used preferentially when working
//	with LUL.
#include <Lucena-Utilities/lulVersion.hpp>


