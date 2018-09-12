/*------------------------------------------------------------------------------------

	Lucena Utilities Library
	“PCH_gsl.hpp”
	Copyright © 2018 Lucena
	All Rights Reserved

	This file is distributed under the University of Illinois Open Source
	License. See license/License.txt for details.

	This header is intended for use as a prefix header, or at least a component
	of one, specifically, but not necessarily, for building precompiled
	headers. We make no assumptions about previously included headers here,
	prefix or otherwise.

------------------------------------------------------------------------------------*/


#pragma once


//	gsl
//	SEEME Well, this is super-easy. Note that we _could_ be more discerning,
//	e.g., by not including <gsl/span> if <span> is being used, but this would
//	have minimal practical effect since many of the obvious candidates end up
//	getting included by other headers anyway. This is a bit like boost, but
//	less grating since the headers are pretty lightweight. Even so, the whole
//	thing should be jettisoned at the earliest opportunity: <span> and <byte>
//	are already in, <gsl/gsl_assert> has been mooted by contracts, <mdspan> is
//	underway to replace <gsl/multispan>, etc.
#include <gsl/gsl>
