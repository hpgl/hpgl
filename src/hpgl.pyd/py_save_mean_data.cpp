/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include "py_mean_data.h"

namespace hpgl
{
	void py_save_mean_data(py_mean_data_t mean_data, const std::string & name, const std::string & property_name)
	{
		FILE * f = fopen(name.c_str(), "w+");
		fprintf(f, "%s\n\n", property_name.c_str());

		size_t size = mean_data.m_data->size();
		for (size_t idx = 0; idx < size; ++idx)
		{
			fprintf(f, "%g ", mean_data.m_data->operator[](idx));
			if (idx % 10 == 0)
				fprintf(f, "\n");
		}
		fprintf(f, "/");
	}
}
