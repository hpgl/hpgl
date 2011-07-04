/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <kriging_stats.h>

namespace hpgl
{
	std::ostream & operator << (std::ostream & stream, kriging_stats_t & stats)
	{
		stream << "Statistics: \n"
			<< "\tNodes: " << stats.m_points_calculated
			<< "\n\tNodes with empty neighbourhood: " << stats.m_points_without_neighbours
			<< "\n\tMean: " << stats.m_mean
			<< "\n\tCalculation performance: " << stats.m_speed_nps
			<< std::endl;

		return stream;
	}
}