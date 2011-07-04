#include "stdafx.h"
#include "neighbourhood_param.h"

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
