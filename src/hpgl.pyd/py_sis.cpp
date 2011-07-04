/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <ik_params.h>
#include <property_array.h>
#include "py_grid.h"
#include <sugarbox_grid.h>
#include "py_parse_sis_params.h"
#include "mean_provider.h"
#include <progress_reporter.h>
#include <hpgl_core.h>
#include "numpy_utils.h"

namespace hpgl
{
	void py_sis(
		boost::python::tuple array, 
		const py_grid_t & grid,
		boost::python::object params, int seed, bool use_vpc, bool use_corellogram, boost::python::object mask_data)
	{
		if (use_vpc)
			throw hpgl_exception("py_sis", "use_vpc no longer supported. Use lvm instead.");

		ik_params_t ik_params;
		parse_sis_params(params, ik_params);

		boost::shared_ptr<indicator_property_array_t> property 
				= ind_prop_from_tuple(array);		
									
		progress_reporter_t report(grid.m_sugarbox_geometry->size());

		unsigned char * mask_ptr = 
			mask_data.ptr() == Py_None ? NULL 
			: get_buffer_from_ndarray<unsigned char,'u'>(mask_data, property->size(), "py_sis");

		sequential_indicator_simulation(
			*property,
			*grid.m_sugarbox_geometry,
			ik_params,
			seed,
			report,
			use_corellogram,
			mask_ptr);
	}



	void py_sis_lvm(
			const boost::python::tuple & array,
			const py_grid_t & grid,
			boost::python::object params,
			int seed,
			boost::python::object mean_data,
			bool use_corellogram,
			boost::python::object mask_data)
	{
		boost::shared_ptr<indicator_property_array_t> property = ind_prop_from_tuple(array);		

		ik_params_t ik_params;
		parse_sis_params(params, ik_params);		
		progress_reporter_t report(grid.m_sugarbox_geometry->size());
		
		unsigned char * mask_ptr =
			mask_data.ptr() == Py_None ? NULL 
			: get_buffer_from_ndarray<unsigned char,'u'>(mask_data, property->size(), "py_sis_lvm");

		std::vector<const mean_t*> means;
		for (int i = 0; i < indicator_count(*property); ++i)
		{
			means.push_back(get_buffer_from_ndarray<mean_t, 'f'>(mean_data[i], property->size(), "py_sis_lvm"));
		}

		sequential_indicator_simulation_lvm(
				*property, 
				*grid.m_sugarbox_geometry, 
				ik_params, seed, 
				&means[0], 
				report, 
				use_corellogram, mask_ptr);	
	}

}
