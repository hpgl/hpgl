/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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