/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __LOOPED_SUGARBOX_GRID_H__1DD7E6FB_4897_4482_AD1B_3A514B3FE6E9
#define __LOOPED_SUGARBOX_GRID_H__1DD7E6FB_4897_4482_AD1B_3A514B3FE6E9

#include "var_radix_utils.h"

namespace hpgl
{
	class looped_sugarbox_grid_t
	{
		int m_x, m_y, m_z;

	public:
		typedef sugarbox_location_t coord_t;
		looped_sugarbox_grid_t(int x, int y, int z)
			: m_x(x), m_y(y), m_z(z)
		{

		}

		coord_t operator[](int index)const
		{
			int x,y,z;
			dec_to_vr(m_y, m_x, index, z, y, x);
			return coord_t(x,y,z);
		}

		int get_index(coord_t coord)const
		{
			int x = (coord[0] + m_x) % m_x;
			int y = (coord[1] + m_y) % m_y;
			int z = (coord[2] + m_z) % m_z;

			return vr_to_dec(m_y, m_x, z, y, x);
		}
	};
}

#endif //__LOOPED_SUGARBOX_GRID_H__1DD7E6FB_4897_4482_AD1B_3A514B3FE6E9
