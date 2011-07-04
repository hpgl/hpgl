/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __CALC_MEAN_H__636E23FA_58B3_4EC3_AB68_F2193290A4EC__
#define __CALC_MEAN_H__636E23FA_58B3_4EC3_AB68_F2193290A4EC__

#include <property_array.h>

namespace hpgl
{
	double calc_mean(const cont_property_array_t & property, bool * success);
}

#endif //__CALC_MEAN_H__636E23FA_58B3_4EC3_AB68_F2193290A4EC__
