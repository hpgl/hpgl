#ifndef __PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__
#define __PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__

#include "property_array.h"

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
		void write_double(const cont_property_array_t & property, double undefined_value);
		void write_byte(sp_byte_property_array_t property, unsigned char undefined_value, const std::vector<indicator_value_t> & remap_table);
		void write_byte(const indicator_property_array_t & property, unsigned char undefined_value, const std::vector<indicator_value_t> & remap_table);

		void write_gslib_double(sp_double_property_array_t property, double undefined_value);
		void write_gslib_byte(sp_byte_property_array_t property, unsigned char undefined_value, const std::vector<indicator_value_t> & remap_table);


	};


}

#endif //__PROPERTY_WRITER_H__59E102CF_3A2E_4784_A0D2_7F6A733EBCDC__
