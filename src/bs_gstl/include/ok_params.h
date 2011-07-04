/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __BS_OK_PARAMS_H__7B5E3BD6_85D0_496D_BFDD_05B0AC7746A0__
#define __BS_OK_PARAMS_H__7B5E3BD6_85D0_496D_BFDD_05B0AC7746A0__

#include <covariance_param.h>
#include <neighbourhood_param.h>

namespace hpgl
{
	class ok_params_t :
			public covariance_param_t,
			public neighbourhood_param_t
	{
	public:

		using neighbourhood_param_t::m_radiuses;
		using neighbourhood_param_t::m_max_neighbours;

		using neighbourhood_param_t::set_radiuses;
	};
	std::ostream & operator <<(std::ostream &, const ok_params_t&);
}

#endif //__BS_OK_PARAMS_H__7B5E3BD6_85D0_496D_BFDD_05B0AC7746A0__
