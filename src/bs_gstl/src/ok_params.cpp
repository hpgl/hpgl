/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <ok_params.h>

namespace hpgl
{
	
	
	std::ostream & operator <<(std::ostream & out, const ok_params_t& param)
	{
		return out << "\tCovariance Type: " << param.m_covariance_type 
	   		<< "\n\tSill: " << param.m_sill
	   		<< "\n\tRanges: [" << param.m_ranges[0] << ", " << param.m_ranges[1] << ", " << param.m_ranges[2] << "]\n"
	   		<< "\tSearch radiuses: " << param.m_radiuses << "\n"
	   		<< "\tMax number of neighbours: " << param.m_max_neighbours << "\n";

	}
	
}

