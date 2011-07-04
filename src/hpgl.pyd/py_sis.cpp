/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <ik_params.h>
#include <property_array.h>
#include "py_sis.h"
#include "py_property_array.h"
#include "py_grid.h"
#include <sugarbox_grid.h>
#include "py_parse_sis_params.h"
#include "mean_provider.h"
#include <progress_reporter.h>
#include "py_mean_data.h"
#include <hpgl_core.h>
#include "extract_lvm_data.h"

namespace hpgl
{
	py_byte_property_array_t py_sis(
			const py_byte_property_array_t & input_array, const py_grid_t & grid,
			PyObject * params, int seed, bool use_vpc, bool use_corellogram, boost::python::object mask_data)
	{
		if (use_vpc)
			throw hpgl_exception("py_sis", "use_vpc no longer supported. Use lvm instead.");

		boost::python::extract<py_byte_property_array_t> mask_ext(mask_data);

		ik_params_t ik_params;
		parse_sis_params(params, ik_params);

		sp_byte_property_array_t output_property(new property_array_t<unsigned char>(*input_array.m_byte_property_array));
		py_byte_property_array_t result;
		result.m_byte_property_array = output_property;
		progress_reporter_t report(grid.m_sugarbox_geometry->size());

		indicator_property_array_t * mask = mask_ext.check() 
			? ((py_byte_property_array_t)mask_ext).m_byte_property_array.get()
			: NULL;

		sequential_indicator_simulation(
			*output_property,
			*grid.m_sugarbox_geometry,
			ik_params,
			seed,
			report,
			use_corellogram,
			mask);
		
		return result;
	}



	py_byte_property_array_t py_sis_lvm(
			const py_byte_property_array_t & input_array,
			const py_grid_t & grid,
			PyObject * params,
			int seed,
			boost::python::object mean_data,
			bool use_corellogram,
			boost::python::object mask_data)
	{
		boost::shared_ptr<indicator_lvm_data_t> lvm_data = extract_lvm_data(mean_data,  indicator_count(*input_array.m_byte_property_array));
		
		ik_params_t ik_params;
		parse_sis_params(params, ik_params);

		sp_byte_property_array_t output_property(new property_array_t<unsigned char>(*input_array.m_byte_property_array));
		py_byte_property_array_t result;
		result.m_byte_property_array = output_property;
		progress_reporter_t report(grid.m_sugarbox_geometry->size());

		boost::python::extract<py_byte_property_array_t> mask_ext(mask_data);
		indicator_property_array_t * mask = mask_ext.check() 
			? ((py_byte_property_array_t)mask_ext).m_byte_property_array.get()

			: NULL;
		sequential_indicator_simulation_lvm(
				*output_property, 
				*grid.m_sugarbox_geometry, 
				ik_params, seed, 
				*lvm_data, 
				report, 
				use_corellogram);	

		return result;
	}

}
