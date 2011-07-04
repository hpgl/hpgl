/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include "py_load_property.h"
#include "property_array.h"
#include "load_property_from_file.h"
#include "../bs_gstl/src/read_gslib.h"
#include "extract_indicator_values.h"


namespace hpgl
{
	py_cont_property_array_t load_cont_property(const std::string & filename, double undefined_value)
	{
		py_cont_property_array_t result;
		result.m_double_property_array.reset(new property_array_t<cont_value_t>(0));
		load_property_from_file(result.m_double_property_array,
				filename, undefined_value);
		return result;
	}

	void transform_values_to_indices(indicator_property_array_t & prop, const std::vector<indicator_value_t> & remap_table)
	{
		assert(indicator_count(prop) == remap_table.size());

		int ic = indicator_count(prop);
		for (int i = 0; i < prop.size(); ++i)
		{
			if (prop.is_informed(i))
			{
				for (int j = 0; j < ic; ++j)
				{
					if (prop[i] == remap_table[j])
					{
						prop.set_at(i, j);
						break;
					}
				}
			}
		}
	}
	
	py_byte_property_array_t load_ind_property(const std::string & filename, int undefined_value, boost::python::list ind_values)
	{
		py_byte_property_array_t result;
		result.m_byte_property_array.reset(new property_array_t<indicator_value_t>(0));
		load_indicator_property_from_file(result.m_byte_property_array,
				filename, (int) undefined_value);
		
		int size = boost::python::len(ind_values);
		set_indicator_count(*result.m_byte_property_array, size);		

		std::vector<indicator_value_t> remap_table;
		extract_indicator_values(ind_values, size, remap_table);
		transform_values_to_indices(*result.m_byte_property_array, remap_table);

		return result;
	}

	py_cont_property_array_t load_gslib_cont_property(const std::string & filename, double undefined_value, int i_size, int j_size, int k_size)
	{
		py_cont_property_array_t result;
		result.m_double_property_array.reset(new property_array_t<cont_value_t>(0));
		load_gslib_cont_property_from_file(result.m_double_property_array,
				filename, undefined_value, i_size, j_size, k_size);
		return result;
	}
	
	py_byte_property_array_t load_gslib_ind_property(const std::string & filename, int undefined_value, boost::python::list ind_values, int i_size, int j_size, int k_size)
	{
		py_byte_property_array_t result;
		result.m_byte_property_array.reset(new property_array_t<indicator_value_t>(0));
		load_gslib_indicator_property_from_file(result.m_byte_property_array,
				filename, (int) undefined_value, i_size, j_size, k_size);

		int size = boost::python::len(ind_values);
		set_indicator_count(*result.m_byte_property_array, size);			

		std::vector<indicator_value_t> remap_table;
		extract_indicator_values(ind_values, size, remap_table);
		transform_values_to_indices(*result.m_byte_property_array, remap_table);

		return result;
	}

}