/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_LOAD_PROPERTY_H__D2E168A9_47F6_4A58_B6CD_70C83117E362
#define __PY_LOAD_PROPERTY_H__D2E168A9_47F6_4A58_B6CD_70C83117E362

#include "py_property_array.h"

namespace hpgl
{
	py_cont_property_array_t load_cont_property(const std::string & filenames, double undefined_value);
	py_byte_property_array_t load_ind_property(const std::string & filename, int undefined_value, boost::python::list indicator_values);


	py_cont_property_array_t load_gslib_cont_property(const std::string & filename, double undefined_value, int i_size, int j_size, int k_size);
	py_byte_property_array_t load_gslib_ind_property(const std::string & filename, int undefined_value, boost::python::list indicator_values, int i_size, int j_size, int k_size);

}

#endif //__PY_LOAD_PROPERTY_H__D2E168A9_47F6_4A58_B6CD_70C83117E362