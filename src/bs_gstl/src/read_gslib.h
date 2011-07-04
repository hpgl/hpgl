/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef LOAD_PROPERTY_GSLIB_FROM_FILE_H_INCLUDED
#define LOAD_PROPERTY_GSLIB_FROM_FILE_H_INCLUDED

#include <typedefs.h>
#include <property_array.h>

namespace hpgl
{

	void 
	load_gslib_cont_property_from_file(
			sp_double_property_array_t property_array, 
			const std::string & file_name, 
			double undefined_value,
			int i_size, int j_size, int k_size);


	void load_gslib_indicator_property_from_file(
			sp_byte_property_array_t property_array, 
			const std::string & file_name, 
			int undefined_value,
			int i_size, int j_size, int k_size);

}// hpgl namespace


#endif // LOAD_PROPERTY_GSLIB_FROM_FILE_H_INCLUDED