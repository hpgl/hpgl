/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_MEDIAN_IK_H__A0D5ED65_54BC_419B_B523_723C47E0CF32
#define __PY_MEDIAN_IK_H__A0D5ED65_54BC_419B_B523_723C47E0CF32

#include <median_ik.h>

#include "numpy_utils.h"

namespace hpgl
{
	class py_median_ik_params_t
	{
	public:
		median_ik_params m_params;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
		void set_marginal_probs(double p1, double p2);	
	};

	void py_median_ik_params_t::set_covariance_type(int type)
	{
		m_params.m_covariance_type = (covariance_type_t) type;
	}

	void py_median_ik_params_t::set_ranges(double r1, double r2, double r3)
	{
		m_params.set_ranges(r1, r2, r3);
	}

	void py_median_ik_params_t::set_angles(double a1, double a2, double a3)
	{
		m_params.set_angles(a1, a2, a3);
	}

	void py_median_ik_params_t::set_sill(double sill)
	{
		m_params.m_sill = sill;
	}

	void py_median_ik_params_t::set_nugget(double nugget)
	{
		m_params.m_nugget = nugget;
	}

	void py_median_ik_params_t::set_radiuses(size_t s1, size_t s2, size_t s3)
	{
		m_params.set_radiuses(s1, s2, s3);
	}
	
	void py_median_ik_params_t::set_max_neighbours(size_t mn)
	{
		m_params.m_max_neighbours = mn;
	}

	void py_median_ik_params_t::set_marginal_probs(double p1, double p2)
	{
		m_params.m_marginal_probs[0] = p1;
		m_params.m_marginal_probs[1] = p2;
	}

	
	py_median_ik_params_t py_create_median_ik_params()
	{
		return py_median_ik_params_t();
	}

	void py_median_ik(boost::python::tuple input_property,
		boost::python::tuple output_property,
		const py_grid_t & grid,
		const py_median_ik_params_t & params)
	{
		using namespace boost::python;
		sp_byte_property_array_t in_prop = ind_prop_from_tuple(input_property);
		sp_byte_property_array_t out_prop = ind_prop_from_tuple(output_property);
			
		std::cout << "Performing Median IK.\n" << params.m_params;

		median_ik_for_two_indicators(params.m_params, *grid.m_sugarbox_geometry, *in_prop, *out_prop);		
	}
}

#endif //__PY_MEDIAN_IK_H__A0D5ED65_54BC_419B_B523_723C47E0CF32