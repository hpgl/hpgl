/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_SIS_H__76857FA3_A4CC_4829_BA7E_98E05C7C6C2B__
#define __PY_SIS_H__76857FA3_A4CC_4829_BA7E_98E05C7C6C2B__

#include "py_lvm_data.h"

namespace hpgl
{
	class py_byte_property_array_t;
	class py_grid_t;

	py_byte_property_array_t py_sis(const py_byte_property_array_t & input_array,
		const py_grid_t & grid,
		PyObject * params,
		int seed,
		bool use_vpc,
		bool use_corellogram,
		boost::python::object mask);	

	py_byte_property_array_t py_sis_lvm(
		const py_byte_property_array_t & input_array,
		const py_grid_t & grid,
		PyObject * params,
		int seed,
		boost::python::object mean_data,
		//const py_indicator_lvm_data_t & mean_data,
		bool use_corellogram,
		boost::python::object mask);

}

#endif //__PY_SIS_H__76857FA3_A4CC_4829_BA7E_98E05C7C6C2B__
