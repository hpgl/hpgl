/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef CLASTERIZER_H_INCLUDED_ASDJLASDJLASDJALSDJLAKSDLASJD
#define CLASTERIZER_H_INCLUDED_ASDJLASDJLASDJALSDJLAKSDLASJD


#include <typedefs.h>
#include <property_array.h>
#include <search_ellipsoid.h>

#include "geometry.h"

namespace hpgl
{

const int MAGIC_NUMBER = 250;
	

class sugarbox_grid_t;

class clusterizer_t
{
	struct state;
	state * m_state;
	typedef location_3d<int> cluster_coord_t;
public:
	clusterizer_t();
	clusterizer_t(
			const sugarbox_grid_t * grid, 			
			const sugarbox_search_ellipsoid_t & ellipsoid,
			size_t limit);
	sugarbox_search_ellipsoid_t m_ellipsoid;
	~clusterizer_t();

public:

	template<typename point_t>
	int get_index_from_grid_point(const point_t & loc)
	{
		int cx = loc[0] / m_ellipsoid[0];
		int cy = loc[1] / m_ellipsoid[0];
		int cz = loc[2] / m_ellipsoid[0];
		return get_index(cx, cy, cz);		
	}

	int get_index(int cx, int cy, int cz)const;
	void add_node(node_index_t idx);
	int get_nearby_harddata_count(node_index_t idx)const;
	bool get_nearby_harddata(node_index_t idx, std::vector<node_index_t> & neighbours)const;
	bool limit_exceeded(node_index_t idx)const;
};

} //namespace hpgl

#endif //CLASTERIZER_H_INCLUDED_ASDJLASDJLASDJALSDJLAKSDLASJD
