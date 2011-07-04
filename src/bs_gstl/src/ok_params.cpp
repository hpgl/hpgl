/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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

