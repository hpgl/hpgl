/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include "locale_keeper.h"
#include "hpgl_exception.h"

namespace hpgl
{
	namespace {
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

		void load_floats_into_vector(FILE * file, float * data, int size)
		{
			char buffer[256];
			for (int i = 0; i < size; ++i)
			{		
start:
				fscanf(file, "%255s", buffer);
				if (ferror(file))
					throw hpgl_exception("load_floats_into_vector",
						"Error reading file.");
			
				size_t len = strlen(buffer);
				if (len >= 2 && buffer[0] == '-' && buffer[1] == '-')
				{
					//comment - skipping rest of line
					fscanf(file, "%*[^\n]");
					goto start;
				}
				if (len >= 1 && buffer[0] == '/')
				{
					throw hpgl_exception("load_floats_into_vector",
						"Unexpected end of data.");					
				}
				else
				{
					float value;	
					if (sscanf(buffer, "%f", &value) != 1)
					{
						throw hpgl_exception("load_floats_into_vector", 
							boost::format("Error parsing '%s' string.") % buffer);
					}					
					data[i] = value;											
				}		
			};
		}		

		void read_bytes(FILE * file, 
			int undefined_value,
			unsigned char * data, 
			unsigned char * mask,			
			int size)
		{
			char buffer[256];
			for (int i = 0; i < size; ++i)
			{		
start:
				fscanf(file, "%255s", buffer);
				if (ferror(file))
					throw hpgl_exception("read_bytes",
						"Error reading file.");
			
				size_t len = strlen(buffer);
				if (len >= 2 && buffer[0] == '-' && buffer[1] == '-')
				{
					//comment - skipping rest of line
					fscanf(file, "%*[^\n]");
					goto start;
				}
				if (len >= 1 && buffer[0] == '/')
				{
					throw hpgl_exception("read_bytes",
						"Unexpected end of data.");					
				}
				else
				{
					int value;	
					if (sscanf(buffer, "%d", &value) != 1)
					{
						throw hpgl_exception("load_floats_into_vector", 
							boost::format("Error parsing '%s' string.") % buffer);
					}					
					data[i] = value;											
					mask[i] = value == undefined_value ? 0 : 1;
				}		
			};
		}	
	}

	void read_inc_file_float(
			const char * file_name,
			float undefined_value,
			int size,
			float * data_buffer,
			unsigned char * mask_buffer)
	{
		blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
		FILE * file = fopen(file_name, "r");
		if (file == 0)
		{
			throw hpgl_exception("read_inc_file_float", std::string("Error opening file:") + file_name + ".");
		}
		std::string prop_name;
		read_prop_name(file, prop_name);
			
		load_floats_into_vector(file, data_buffer, size);

		if (mask_buffer != 0)
		{
			for (int i = 0; i < size; ++i)
			{
				mask_buffer[i] = data_buffer[i] == undefined_value ? 0 : 1;
			}
		}
	}

	void read_inc_file_byte(
		const char * file_name,
		int undefined_value,
		int size,
		unsigned char * data_buffer,
		unsigned char * mask_buffer)
	{
		blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
		FILE * file = fopen(file_name, "r");
		if (file == 0)
		{
			throw hpgl_exception("read_inc_file_float", std::string("Error opening file:") + file_name + ".");
		}

		std::string prop_name;
		read_prop_name(file, prop_name);
			
		read_bytes(file, undefined_value, data_buffer, mask_buffer, size);
	}
}