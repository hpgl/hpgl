/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include "py_lvm_data.h"
#include "load_property_from_file.h"
#include "typedefs.h"

namespace hpgl
{
	py_indicator_lvm_data_t py_load_indicator_mean_data(PyObject * filenames)
	{
		size_t file_count = PyList_Size(filenames);
		std::vector<boost::shared_ptr<std::vector<mean_t> > > data(file_count);
		for (size_t i = 0; i < file_count; ++i)
		{
			std::string file_name =  PyString_AsString(PyList_GetItem(filenames, i));
			data[i].reset(new std::vector<mean_t>());
			load_variable_mean_from_file(*data[i], file_name);
		}

		py_indicator_lvm_data_t result;
		result.m_lvm_data.reset(new indicator_lvm_data_t());
		result.m_lvm_data->assign(data);
		return result;
	}
}