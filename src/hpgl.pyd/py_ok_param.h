/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__
#define __PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__

#include <ok_params.h>

namespace hpgl
{
	class py_ok_params_t
	{
	public:
		ok_params_t m_ok_param;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
	};
	
}

#endif //__PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__
