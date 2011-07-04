/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <load_property_from_file.h>
#include <typedefs.h>

#include "py_mean_data.h"

namespace hpgl
{

py_mean_data_t py_load_mean_data(const std::string & file_name)
{
	py_mean_data_t result;
	result.m_data.reset(new std::vector<mean_t>());
	load_variable_mean_from_file(*result.m_data,
		file_name);
	return result;
}


}