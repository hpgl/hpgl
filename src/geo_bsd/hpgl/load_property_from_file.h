#ifndef LOAD_PROPERTY_FROM_FILE_H_INCLUDED
#define LOAD_PROPERTY_FROM_FILE_H_INCLUDED

#include "typedefs.h"
#include "property_array.h"

namespace hpgl
{


void
load_property_from_file(
		sp_double_property_array_t property_array,
		const std::string & file_name, double undefined_value);


void load_indicator_property_from_file(
		sp_byte_property_array_t property_array,
		const std::string & file_name,
		int undefined_value);
		

void load_variable_mean_from_file(
		std::vector<mean_t> & data,
		const std::string & file_name);



}


#endif // LOAD_PROPERTY_FROM_FILE_H_INCLUDED
