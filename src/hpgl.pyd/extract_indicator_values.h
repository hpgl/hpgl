/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __EXTRACT_INDICATOR_VALUES_H__983CBD36_7A03_465D_8962_748DD91A6187
#define __EXTRACT_INDICATOR_VALUES_H__983CBD36_7A03_465D_8962_748DD91A6187

namespace hpgl
{
	void extract_indicator_values(
		boost::python::list values,
		int expected_count,
		std::vector<indicator_value_t> & result);
}

#endif //__EXTRACT_INDICATOR_VALUES_H__983CBD36_7A03_465D_8962_748DD91A6187