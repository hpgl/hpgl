/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef KRIGING_STATS_H_INCLUDED_93485F0934MNKSDHKFANQ29F1IOEMNFIOSDAHFRQ23G4UJASDFDOBJRH21FSD
#define KRIGING_STATS_H_INCLUDED_93485F0934MNKSDHKFANQ29F1IOEMNFIOSDAHFRQ23G4UJASDFDOBJRH21FSD

namespace hpgl
{
	struct kriging_stats_t
	{
		unsigned long m_points_calculated;
		unsigned long m_points_without_neighbours;
		double m_mean;
		double m_speed_nps;
	};

	std::ostream & operator << (std::ostream & stream, kriging_stats_t & stats);
}

#endif //KRIGING_STATS_H_INCLUDED_93485F0934MNKSDHKFANQ29F1IOEMNFIOSDAHFRQ23G4UJASDFDOBJRH21FSD