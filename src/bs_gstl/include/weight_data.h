/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __WEIGHT_DATA_H__89F944E4_A281_4E3A_810E_6CDE8DC4BA44
#define __WEIGHT_DATA_H__89F944E4_A281_4E3A_810E_6CDE8DC4BA44

#include <typedefs.h>

namespace hpgl
{
	typedef int worker_id_t;
	struct weight_data_t
	{
		int count;
		std::vector<node_index_t> indices;
		std::vector<kriging_weight_t> weights;		
	};
}

#endif //__WEIGHT_DATA_H__89F944E4_A281_4E3A_810E_6CDE8DC4BA44