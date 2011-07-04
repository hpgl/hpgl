/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <hpgl_core.h>
#include "hpgl_exception.h"
#include "numpy_utils.h"
#include "extract_indicator_values.h"

namespace hpgl
{
	namespace python
	{
		namespace
		{
			void transform_values_to_indices(
				indicator_value_t * prop,
				unsigned char * mask,
				int size,
				const std::vector<indicator_value_t> & remap_table)
			{
				int ic = (int) remap_table.size();
				for (int i = 0; i < size; ++i)
				{
					if (mask[i] != 0)
					{
						for (int j = 0; j < ic; ++j)
						{
							if (prop[i] == remap_table[j])
							{
								prop[i] = j;
								break;
							}
						}
					}
				}
			}
		}

		void py_read_inc_file_float(
			const std::string & filename,
			double undefined_value,
			int size,
			boost::python::object odata,
			boost::python::object omask)
		{
			using namespace std;
			using namespace boost;
			using namespace boost::python;
			
			
			float * data_ptr = get_buffer_from_ndarray<float, 'f'>(odata, size, "py_read_inc_file_float");			
			unsigned char * mask_ptr = get_buffer_from_ndarray<unsigned char, 'u'>(omask, size, "py_read_inc_file_float");			

			read_inc_file_float(filename.c_str(), (float)undefined_value, size, data_ptr, mask_ptr);
		}

		void py_read_inc_file_byte(
			const std::string & filename,
			int undefined_value,
			int size,
			boost::python::object data,
			boost::python::object mask, 
			boost::python::list ind_values)
		{
			unsigned char * data_ptr = get_buffer_from_ndarray<unsigned char, 'u'>(data, size, "py_read_inc_file_byte");
			unsigned char * mask_ptr = get_buffer_from_ndarray<unsigned char, 'u'>(mask, size, "py_read_inc_file_byte");
			read_inc_file_byte(filename.c_str(), undefined_value, size, data_ptr, mask_ptr);

			std::vector<indicator_value_t> remap_table;
			extract_indicator_values(ind_values, (int) boost::python::len(ind_values), remap_table);
			transform_values_to_indices(data_ptr, mask_ptr, size, remap_table);
		}
	}
}