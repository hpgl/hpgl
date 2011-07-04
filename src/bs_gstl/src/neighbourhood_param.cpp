/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <neighbourhood_param.h>

namespace hpgl
{
	neighbourhood_param_t::neighbourhood_param_t()
		: 
	m_max_neighbours(0)
	{
		set_radiuses(0, 0, 0);
	}

	neighbourhood_param_t::neighbourhood_param_t(const sugarbox_search_ellipsoid_t & radiuses, int max_neighbours)
		: m_radiuses(radiuses),
		m_max_neighbours(max_neighbours)
	{
	}

	void neighbourhood_param_t::set_radiuses(size_t radius1, size_t radius2, size_t radius3)
	{
		m_radiuses[0] = radius1;
		m_radiuses[1] = radius2;
		m_radiuses[2] = radius3;
	}
}
