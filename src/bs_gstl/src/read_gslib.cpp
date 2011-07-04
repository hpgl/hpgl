/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/



#include "stdafx.h"
#include "read_gslib.h"
#include "locale_keeper.h"


#include <property_array.h>
#include "hpgl_exception.h"

namespace hpgl
{

	void load_gslib_property(const std::string & file_name, std::vector<indicator_value_t> & data)
	{
		blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
		FILE * file = fopen(file_name.c_str(), "r");
		if (file == 0)
		{
			throw hpgl_exception("load_gslib_property", std::string("Error opening file:") + file_name + ".");
		}
		
		char temp_buff[1024];
		
		// 1. Property name
		std::string title;
		fscanf(file, "%1023[^\n]\n", temp_buff);
		title = temp_buff;

	//	std::cout << "Property name:" << title << std::endl;

		// 2. Variables Number
		int var_num = 0;
		fscanf(file, "%d", &var_num);

	//	std::cout << "Var num:" << var_num << std::endl;

		if (var_num != 1)
		{
			throw hpgl_exception("load_gslib_property", std::string("Sorry, only one-variable GSLIB files are supported."));
		}

		// 3. Variable Name
		std::string prop_name;
		fscanf(file, "%s\n", temp_buff);
		prop_name = temp_buff;

	//	std::cout << "Var name:" << prop_name << std::endl;

		// 4. Data Values

	//	std::cout << "Reading..." << std::endl;

		int temp_var = 0;
		while(fscanf(file, "%s", temp_buff)!=EOF)
		{
			if(sscanf(temp_buff, "%d", &temp_var))
			{
				data.push_back(temp_var);
				//std::cout << temp_var << std::endl;
			}
		}

	//	std::cout << "Readed" << std::endl;
	}

	void load_gslib_property(const std::string & file_name, std::vector<cont_value_t> & data)
	{
		blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
		FILE * file = fopen(file_name.c_str(), "r");
		if (file == 0)
		{
			throw hpgl_exception("load_gslib_property", std::string("Error opening file:") + file_name + ".");
		}
		
		char temp_buff[1024];
		
		// 1. Property name
		std::string title;
		fscanf(file, "%1023[^\n]\n", temp_buff);
		title = temp_buff;
		
		// 2. Variables Number
		int var_num = 0;
		fscanf(file, "%d", &var_num);


		if (var_num != 1)
		{
			throw hpgl_exception("load_gslib_property", std::string("Sorry, only one-variable GSLIB files are supported."));
		}

		// 3. Variable Name
		std::string prop_name;
		fscanf(file, "%s\n", temp_buff);
		prop_name = temp_buff;

		// 4. Data Values
		float temp_var = 0;
		while(fscanf(file, "%s", temp_buff)!=EOF)
		{
			if(sscanf(temp_buff, "%f", &temp_var))
			{
				data.push_back(temp_var);
				//std::cout << temp_var << std::endl;
			}
		}

	}

	// GSLIB

	void 
	load_gslib_cont_property_from_file(
			sp_double_property_array_t property_array, 
			const std::string & file_name, 
			double undefined_value,
			int i_size, int j_size, int k_size)
	{	

		std::vector<cont_value_t> data;
		std::vector<cont_value_t> data_reshaped;

		load_gslib_property(file_name, data);

		// reshaping

		int position = 0;

		for(int k=0; k<k_size; k++)
			for(int j=0; j<j_size; j++)
				for(int i=0; i<i_size; i++)
				{
					position = ((k_size-1)-k) * i_size * j_size + ((j_size-1)-j) * i_size + i;
					
					if(position >= data.size())
						throw hpgl_exception("load_gslib_cont_property_from_file", std::string("Not enough property points for defined I,J,K values"));

					data_reshaped.push_back(data[position]);
				}
		
		property_array->assign(data_reshaped, undefined_value);
		//property_array->set_ijk(i_size,j_size,k_size);
	}


	void load_gslib_indicator_property_from_file(
			sp_byte_property_array_t property_array, 
			const std::string & file_name, 
			int undefined_value,
			int i_size, int j_size, int k_size)
	{

		std::vector<indicator_value_t> data;
		std::vector<indicator_value_t> data_reshaped;

		load_gslib_property(file_name, data);
		
		// reshaping

		int position = 0;

		for(int k=0; k<k_size; k++)
			for(int j=0; j<j_size; j++)
				for(int i=0; i<i_size; i++)
				{
					position = ((k_size-1)-k) * i_size * j_size + ((j_size-1)-j) * i_size + i;

					if(position >= data.size())
						throw hpgl_exception("load_gslib_indicator_property_from_file", std::string("Not enough property points for defined I,J,K values"));

					data_reshaped.push_back(data[position]);
				}

		property_array->assign(data_reshaped, undefined_value);
		//property_array->set_ijk(i_size,j_size,k_size);

	}

} // namespace hgpl