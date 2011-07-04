/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include "py_grid.h"
#include <ik_params.h>
#include <property_array.h>
#include <hpgl_exception.h>
#include <hpgl_core.h>

#include "numpy_utils.h"

namespace hpgl
{
	const char * c_param_cov_type = "cov_type";
	const char * c_param_ranges = "ranges";
	const char * c_param_angles = "angles";
	const char * c_param_sill = "sill";
	const char * c_param_nugget = "nugget";
	const char * c_param_radiuses = "radiuses";
	const char * c_param_max_neighbours = "max_neighbours";
	const char * c_param_marg_prob = "marginal_prob";
	const char * c_param_value = "value";

	bool is_valid_parameter(const std::string param_name)
	{
		return 
			param_name == c_param_cov_type ||
			param_name == c_param_ranges ||
			param_name == c_param_angles ||
			param_name == c_param_sill ||
			param_name == c_param_nugget ||
			param_name == c_param_radiuses ||
			param_name == c_param_max_neighbours ||
			param_name == c_param_marg_prob ||
			param_name == c_param_value; //left for backward compatibility. "value" parameter not used anymore.
	}

	void check_parameters(PyObject * params_dictionary)
	{
		PyObject * keys = PyDict_Keys(params_dictionary);
		Py_ssize_t idx, size;
		size = PyList_Size(keys);
		for (idx = 0; idx < size; ++idx)
		{
			char * key = PyString_AsString(PyList_GetItem(keys, idx));
			if (!is_valid_parameter(key))
				throw hpgl_exception("py_indicator_kriging", std::string("Invalid ik parameter '") + key + "'.");
		}		
	}


	void py_indicator_kriging(
		boost::python::tuple input_array,
		boost::python::tuple output_array,
		const py_grid_t & grid,
		PyObject * params, 
		bool use_vpc)
{
	using namespace hpgl;
	if (use_vpc)
		throw hpgl_exception("py_indicator_kriging.", "use_vpc is no longer supported. use lvm instead");
	ik_params_t ik_params;
	Py_ssize_t idx, end_idx;
	end_idx = PyList_Size(params);
	for (idx = 0; idx < end_idx; ++idx)
	{
		indicator_param_t indicator_param;
		PyObject * ik_dict              = PyList_GetItem(params, idx);
		check_parameters(ik_dict);
		PyObject * cov_type             = PyDict_GetItemString(ik_dict, c_param_cov_type);
		PyObject * ranges               = PyDict_GetItemString(ik_dict, c_param_ranges);
		PyObject * angles               = PyDict_GetItemString(ik_dict, c_param_angles);
		PyObject * sill                 = PyDict_GetItemString(ik_dict, c_param_sill);
		PyObject * nugget               = PyDict_GetItemString(ik_dict, c_param_nugget);
		PyObject * radiuses             = PyDict_GetItemString(ik_dict, c_param_radiuses);
		PyObject * max_neighbours       = PyDict_GetItemString(ik_dict, c_param_max_neighbours);
		PyObject * marginal_probability = PyDict_GetItemString(ik_dict, c_param_marg_prob);
		PyObject * value                = PyDict_GetItemString(ik_dict, c_param_value);

		if (cov_type)
			indicator_param.m_covariance_type = (covariance_type_t) PyInt_AsLong(cov_type);
		if (ranges)
		{
			double r1 = PyFloat_AsDouble(PyTuple_GetItem(ranges, 0));
			double r2 = PyFloat_AsDouble(PyTuple_GetItem(ranges, 1));
			double r3 = PyFloat_AsDouble(PyTuple_GetItem(ranges, 2));
			indicator_param.set_ranges(r1, r2, r3);
		} 
		if (angles)
		{
			double a1 = PyFloat_AsDouble(PyTuple_GetItem(angles, 0));
			double a2 = PyFloat_AsDouble(PyTuple_GetItem(angles, 1));
			double a3 = PyFloat_AsDouble(PyTuple_GetItem(angles, 2));
			indicator_param.set_angles(a1, a2, a3);
		}
		if (sill)
			indicator_param.m_sill = PyFloat_AsDouble(sill);
		if (nugget)
			indicator_param.m_nugget = PyFloat_AsDouble(nugget);
		if (radiuses)
		{
			int r1 = PyInt_AsLong(PyTuple_GetItem(radiuses, 0));
			int r2 = PyInt_AsLong(PyTuple_GetItem(radiuses, 1));
			int r3 = PyInt_AsLong(PyTuple_GetItem(radiuses, 2));
			indicator_param.set_radiuses(r1, r2, r3);
		}
		if (max_neighbours)
			indicator_param.m_max_neighbours = PyInt_AsLong(max_neighbours);
		if (marginal_probability)
			indicator_param.m_marginal_prob = PyFloat_AsDouble(marginal_probability);		
		ik_params.add_indicator(indicator_param);
	}

	sp_byte_property_array_t in_prop = ind_prop_from_tuple(input_array);
	sp_byte_property_array_t out_prop = ind_prop_from_tuple(output_array);
	
	indicator_kriging(*in_prop, *grid.m_sugarbox_geometry, ik_params, *out_prop);	
}

} //namespace hpgl
