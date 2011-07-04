/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__
#define __PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__

#include <sgs_params.h>

namespace hpgl
{
	class py_sgs_params_t
	{
	public:
		sgs_params_t m_sgs_params;		
		bool m_auto_region_size;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
		void set_mean(double mean);
		void set_kriging_kind(int kind);
		void set_seed(long int seed);
		void set_mean_kind(const std::string & mean_kind);		
		void set_min_neighbours(long int min_n);
	};
}

#endif //__PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__
