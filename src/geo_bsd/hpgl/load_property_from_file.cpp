#include "stdafx.h"

#include "load_property_from_file.h"
#include "property_array.h"
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

