/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <omp.h>

namespace hpgl
{
	void set_thread_num(int n_threads)
	{
		omp_set_num_threads(n_threads);
	}
}