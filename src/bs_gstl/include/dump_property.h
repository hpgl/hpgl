/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __DUMP_PROPERTY_H__2FBEE622_2D13_466D_97DE_6BF6F900F8F7__
#define __DUMP_PROPERTY_H__2FBEE622_2D13_466D_97DE_6BF6F900F8F7__

#include <get_compile_time.h>

namespace hpgl
{
		template <typename T, typename Ostream>
		void dump_property(const property_array_t<T> & property,
			T undefined_value,
			const std::string & name,
				Ostream & stream)
		{
			stream << name << '\n';
			for (int idx = 0, end_idx = property->size();
				idx < end_idx; ++idx)
			{
				if (property->is_informed(idx))
					stream << (double) property->get_at(idx) << '\n';
				else
					stream << (double) undefined_value << '\n';
			}
			stream.flush();
		}

		template <typename T>
		void dump_property_to_file(const property_array_t<T> & property,
			T undefined_value, const std::string & filename)
		{
			std::ofstream stream((filename + "_"__DATE__"_" + get_compile_time()).c_str());
			dump_property(property, undefined_value, "PROPERTY", stream);
			stream.close();
		}


}


#endif //__DUMP_PROPERTY_H__2FBEE622_2D13_466D_97DE_6BF6F900F8F7__
