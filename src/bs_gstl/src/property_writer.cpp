/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <property_array.h>
#include <property_writer.h>
#include "locale_keeper.h"
#include "hpgl_exception.h"

namespace hpgl
{
	void property_writer_t :: init(
		const std::string & filename,
		const std::string& property_name)
	{
		m_file_name = filename;
		m_property_name = property_name;
	}
	
	void write_value(FILE * f, unsigned char value)
	{
		fprintf(f, "%d\n", (int)(char)value );
	}

	void write_value(FILE * f, double value)
	{
		fprintf(f, "%E\n", value);
	}

	namespace {	

		typedef boost::shared_ptr<FILE> file_t;
		file_t open_file_checked(const char * filename, const char * mode)
		{
			FILE * f = fopen(filename, mode);
			if (f == 0)
				throw hpgl_exception("open_file_checked", boost::format("Can't open file '%s'.") % filename);
			return file_t(f, fclose);
		}

		void write_property_cont(
				const char * filename, 
				const char * property_name,
				const cont_property_array_t & property,
				cont_value_t undefined_value
				)
		{
			blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
			file_t f = open_file_checked(filename, "w+");			
			fprintf(f.get(), "%s\n", property_name);

			for (int i = 0, end_i = property.size(); i < end_i; ++i)
			{
				if (property.is_informed(i))						
					write_value(f.get(), property.get_at(i));
				else
					write_value(f.get(), undefined_value);
			}

			fprintf(f.get(), "/\n");			
		}

		void write_property_ind(
				const char * filename, 
				const char * property_name,
				const indicator_property_array_t & property,
				indicator_value_t undefined_value,
				const std::vector<indicator_value_t> & remap_table
				)
		{
			blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
			file_t f = open_file_checked(filename, "w+");			
			fprintf(f.get(), "%s\n", property_name);

			for (int i = 0, end_i = property.size(); i < end_i; ++i)
			{
				if (property.is_informed(i))	
					write_value(f.get(), remap_table[property.get_at(i)]);
				else
					write_value(f.get(), undefined_value);
			}

			fprintf(f.get(), "/\n");			
		}
	}

	namespace {
		void write_header(FILE * f,int var_num, const char * property_name)
		{			
			fprintf(f, "HPGL saved GSLIB file\n");			
			fprintf(f, "%d\n", var_num);
			fprintf(f, "%s\n", property_name);
		}

		void write_gslib_property_cont_c(
				const char * filename, 
				const char * property_name,
				const cont_property_array_t & property,
				cont_value_t undefined_value,
				int i_size, int j_size, int k_size
				)
		{
			blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
			file_t f = open_file_checked(filename, "w+");			

			int var_num = 1;
			write_header(f.get(), var_num, property_name);

			int position = 0;
			for(int k=0; k<k_size; k++)
				for(int j=0; j<j_size; j++)
					for(int i=0; i<i_size; i++)
				{
					position = ((k_size-1)-k) * i_size * j_size + ((j_size-1)-j) * i_size + i;
				
					if (property.is_informed(position))	
						write_value(f.get(), property.get_at(position));
					else
						write_value(f.get(), undefined_value);
				}			
		}

		void write_gslib_property_ind_c(
				const char * filename, 
				const char * property_name,
				const indicator_property_array_t & property,
				indicator_value_t undefined_value,
				int i_size, int j_size, int k_size,
				const std::vector<indicator_value_t> & remap_table
				)
		{
			blue_sky::locale_keeper lkeeper ("C", LC_NUMERIC);
			file_t f = open_file_checked(filename, "w+");			

			int var_num = 1;
			write_header(f.get(), var_num, property_name);

			int position = 0;
			for(int k=0; k<k_size; k++)
				for(int j=0; j<j_size; j++)
					for(int i=0; i<i_size; i++)
				{
					position = ((k_size-1)-k) * i_size * j_size + ((j_size-1)-j) * i_size + i;
				
					if (property.is_informed(position))	
						write_value(f.get(), remap_table[property.get_at(position)]);
					else
						write_value(f.get(), undefined_value);
				}			
		}


	}
	void property_writer_t :: write_double(
			sp_double_property_array_t property, 
			double undefined_value)
	{
		write_property_cont(
				m_file_name.c_str(), 
				m_property_name.c_str(),
				*property,
				undefined_value);		
	}

	void property_writer_t :: write_byte(
			sp_byte_property_array_t property, 
			unsigned char undefined_value,
			const std::vector<unsigned char> & remap_table)
	{
		write_property_ind(
			m_file_name.c_str(), 
			m_property_name.c_str(),
			*property,
			undefined_value,
			remap_table);		
	}

		void write_gslib_double(
			sp_double_property_array_t property, 
			double undefined_value,
			const char * filename,
			const char * prop_name,
			int i_size, int j_size, int k_size)
	{
		write_gslib_property_cont_c(
				filename, 
				prop_name,
				*property,
				undefined_value,
				i_size, j_size, k_size);		
	}

	void write_gslib_byte(
			sp_byte_property_array_t property, 
			unsigned char undefined_value,
			const char * filename,
			const char * prop_name,
			int i_size, int j_size, int k_size,
			const std::vector<indicator_value_t> & remap_table)
	{
		write_gslib_property_ind_c(
			filename, 
			prop_name,
			*property,
			undefined_value,
			i_size, j_size, k_size, remap_table);		
	}
}