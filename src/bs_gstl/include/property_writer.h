/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__
#define __PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__

#include <property_array.h>

namespace hpgl
{
	class property_writer_t
	{
		std::string m_property_name;
		std::string m_file_name;

	public:
		property_writer_t(){}

		void init(
				const std::string& filename,
				const std::string& property_name);


		void write_double(sp_double_property_array_t property, double undefined_value);
		void write_byte(sp_byte_property_array_t property, unsigned char undefined_value, const std::vector<indicator_value_t> & remap_table);


	};

		void write_gslib_double(sp_double_property_array_t property, double undefined_value, const char * filename, const char * prop_name, int i_size, int j_size, int k_size);
		void write_gslib_byte(sp_byte_property_array_t property, unsigned char undefined_value, const char * filename, const char * prop_name, int i_size, int j_size, int k_size, const std::vector<indicator_value_t> & remap_table);


}

#endif //__PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__
