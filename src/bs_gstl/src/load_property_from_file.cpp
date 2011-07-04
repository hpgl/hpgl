/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <load_property_from_file.h>
#include <property_array.h>
#include "hpgl_exception.h"
#include "locale_keeper.h"

namespace hpgl
{

void read_prop_name(FILE * file, std::string & prop_name)
{
	char line[1024];
	int result = -1;
start:
	line[0] = 0;
	result = fscanf(file, "%1023[^\n]\n", line);
	size_t line_size = strlen(line);	
	if (result == 0 || line_size == 0)
	{
		if (feof(file))
		{
			prop_name = "";
			throw hpgl_exception("read_prop_name", "Property name not found.");
		}
		else
			goto start;
	}
	if (!isalpha(static_cast<unsigned char>(line[0])))
	{
		//skipping line
		while (line_size == 1023)
		{
			if (fscanf(file, "%1023[^\n]\n", line) == 0)
				break;
			line_size = strlen(line);
		}
		goto start;
	}
	else
	{
		// finally line starting with letter
		prop_name = line;
		while (line_size == 1023)
		{
			if (fscanf(file, "%1023[^\n]\n", line) == 0)
				break;
			line_size = strlen(line);
			prop_name += line;
		}
	}
}

bool is_dot(int c)
{
	return c == '.' || c == ',';
}

bool is_ws(int c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\t';
}

bool is_eol(int c)
{
	return c == '\n';
}

bool is_digit(int c)
{
	return 
		c == '0' || 
		c == '1' || 
		c == '2' ||
		c == '3' ||
		c == '4' ||
		c == '5' ||
		c == '6' ||
		c == '7' ||
		c == '8' ||
		c == '9'; 
}

bool is_exp(int c)
{
	return c == 'e' || c == 'E';
}

template <typename T>
void load_doubles_into_vector(FILE * file, std::vector<T> & data)
{
	char buffer[256];
	float value;	
	while (fscanf(file, "%255s", buffer))	
	{
		size_t len = strlen(buffer);
		if (len >= 2 && buffer[0] == '-' && buffer[1] == '-')
		{
			fscanf(file, "%*[^\n]");
			continue;
		}
		if (len >= 1 && buffer[0] == '/')
		{
			break;
		}
		else
		{
			if(sscanf(buffer, "%f", &value))
				data.push_back(value);
		}		
	};
}

void 
load_property_from_file(
		sp_double_property_array_t property_array, 
		const std::string & file_name, double undefined_value)
{	

	blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
	FILE * file = fopen(file_name.c_str(), "r");
	if (file == 0)
	{
		throw hpgl_exception("load_property_from_file", std::string("Error opening file:") + file_name + ".");
	}
	std::string prop_name;
	read_prop_name(file, prop_name);
	
	std::vector<cont_value_t> data;
	
	load_doubles_into_vector(file, data);
	property_array->assign(data, undefined_value);
}

void load_indicator_property_from_file(
		sp_byte_property_array_t property_array, 
		const std::string & file_name, 
		int undefined_value)
{
	blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
	FILE * file = fopen(file_name.c_str(), "r");	
	if (file == 0)
	{
		throw hpgl_exception("load_indicator_property_from_file", std::string("Error opening file:") + file_name + ".");
	}

	std::string prop_name;

	read_prop_name(file, prop_name);

	int value;
	int idx = 0;
	std::vector<indicator_value_t> data;
	while (fscanf(file, "%d\n", &value))
	{
		if (value == undefined_value)
			data.push_back(0xFF);
		else
			data.push_back(indicator_value_t(value));
	};

	property_array->assign(data, 0xFF);
}

void load_variable_mean_from_file(
	std::vector<mean_t> & data,
	const std::string & file_name)
{
	blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
	FILE * file = fopen(file_name.c_str(), "r");	
	if (file == 0)
	{
		throw hpgl_exception("load_variable_mean_from_file", std::string("Error opening file:") + file_name + ".");
	}
	
	std::string prop_name;
	read_prop_name(file, prop_name);		
	load_doubles_into_vector(file, data);	
}

bool is_comment(char * buffer)
{
	return strlen(buffer) >= 2 && buffer[0] == '-' && buffer[1] == '-';
}

}// hpgl namespace

