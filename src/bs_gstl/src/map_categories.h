/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __MAP_CATEGORIES_H__85188541_A46B_492D_9CA8_B64EE3307DF1
#define __MAP_CATEGORIES_H__85188541_A46B_492D_9CA8_B64EE3307DF1

namespace hpgl
{
	template<typename T>
	void map_categories(std::vector<T> & data, std::vector<indicator_value_t> & indicator_values)
	{
		for (int i = 0; i < data.size(); ++i)
		{
			bool mapped = false;
			for (int j = 0; j < indicator_values.size(); ++i)
			{
				if (data[i] == indicator_values[j])
				{
					data[i] = j;
					mapped = true;
					break;
				}				
			}
			if (!mapped)
				throw hpgl_exception("map_categories", boost::format("Unexpected value: %s.") % data[i]);
		}
	}

	template<typename T>
	void map_categories(indicator_property_array_t & data, std::vector<indicator_value_t> & indicator_values)
	{
		for (int i = 0; i < data.size(); ++i)
		{
			bool mapped = false;
			for (int j = 0; j < indicator_values.size(); ++i)
			{
				if (data.get_at(i) == indicator_values[j])
				{
					data.set_at(i, j);
					mapped = true;
					break;
				}				
			}
			if (!mapped)
				throw hpgl_exception("map_categories", boost::format("Unexpected value: %s.") % data[i]);
		}
	}
}

#endif //__MAP_CATEGORIES_H__85188541_A46B_492D_9CA8_B64EE3307DF1