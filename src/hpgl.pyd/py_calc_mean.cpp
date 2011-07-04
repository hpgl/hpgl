/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include "py_property_array.h"
#include "py_mean_data.h"
#include <hpgl_core.h>
#include <hpgl_exception.h>

namespace hpgl
{
	namespace python
	{
		double py_calc_mean(const py_cont_property_array_t & prop)
		{
			double result;
			if (!calc_mean(prop.m_double_property_array.get(), &result))
			{
				throw hpgl_exception("py_calc_mean.", "Error calculating mean.");
			}
			return result;
		}

		double py_calc_mean(const py_mean_data_t & prop)
		{
			double sum = 0;
			for (int i = 0; i < prop.m_data->size(); ++i)
			{
				sum += (*prop.m_data)[i];
			}
			return sum / prop.m_data->size();
		}
	}
}