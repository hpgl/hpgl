/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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