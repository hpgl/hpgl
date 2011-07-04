/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __NEIGHBOURHOOD_PARAM_H__BDE0925A_D03A_4288_8306_3ADE14B0004E__
#define __NEIGHBOURHOOD_PARAM_H__BDE0925A_D03A_4288_8306_3ADE14B0004E__

#include <search_ellipsoid.h>

namespace hpgl
{
	class neighbourhood_param_t
	{
	public:
		neighbourhood_param_t();
		neighbourhood_param_t(const sugarbox_search_ellipsoid_t & radiuses, int max_neighbours);
		sugarbox_search_ellipsoid_t m_radiuses;
		size_t m_max_neighbours;
		void set_radiuses(size_t radius1,
			size_t radius2, size_t radius3);
	};
}

#endif //__NEIGHBOURHOOD_PARAM_H__BDE0925A_D03A_4288_8306_3ADE14B0004E__
