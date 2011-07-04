/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef PY_MEAN_DATA_H_INCLUDED_4RFGVBN0OIKJWEDSFTGHGFTVRUYJH7
#define PY_MEAN_DATA_H_INCLUDED_4RFGVBN0OIKJWEDSFTGHGFTVRUYJH7

#include <typedefs.h>

namespace hpgl
{
	class py_mean_data_t
	{
	public:
		boost::shared_ptr<std::vector<mean_t> > m_data;

	};

	py_mean_data_t py_load_mean_data(const std::string & file_name);

}

#endif //PY_MEAN_DATA_H_INCLUDED_4RFGVBN0OIKJWEDSFTGHGFTVRUYJH7