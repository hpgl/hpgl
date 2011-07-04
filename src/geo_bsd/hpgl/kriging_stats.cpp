#include "stdafx.h"

#include "kriging_stats.h"

namespace hpgl
{
	std::ostream & operator << (std::ostream & stream, const kriging_stats_t & stats)
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
