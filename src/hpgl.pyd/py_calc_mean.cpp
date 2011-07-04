/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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