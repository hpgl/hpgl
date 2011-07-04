/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_SK_PARAMS_H__8F35DFC3_7795_4B82_91A9_353FD9D91053__
#define __PY_SK_PARAMS_H__8F35DFC3_7795_4B82_91A9_353FD9D91053__

#include <sk_params.h>

namespace hpgl
{
	class py_sk_params_t
	{
	public:
		sk_params_t m_sk_params;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
		void set_mean(double mean);
	};
}


#endif //__PY_SK_PARAMS_H__8F35DFC3_7795_4B82_91A9_353FD9D91053__