/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef INCLUDED_MEDIAN_IK_H_IN_SOME_BLUE_SKY_PROJECT_SDFLSDKJFLSDFJLSDKFJLSDF
#define INCLUDED_MEDIAN_IK_H_IN_SOME_BLUE_SKY_PROJECT_SDFLSDKJFLSDFJLSDKFJLSDF

#include <typedefs.h>
#include <sugarbox_grid.h>
#include <property_array.h>
#include <search_ellipsoid.h>
#include <ok_params.h>

namespace hpgl
{

	struct median_ik_params : public ok_params_t
	{			
		double m_marginal_probs[2];
	private:
		indicator_value_t m_values[2];	
	};

void median_ik_for_two_indicators(
		const median_ik_params &, 
		const sugarbox_grid_t & grid,
		const indicator_property_array_t & input_property,
		indicator_property_array_t & output_property
);

}

#endif // INCLUDED_MEDIAN_IK_H_IN_SOME_BLUE_SKY_PROJECT_SDFLSDKJFLSDFJLSDKFJLSDF

