/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include "py_ok_param.h"

namespace hpgl
{
	void py_ok_params_t :: set_covariance_type(int type)
	{
		m_ok_param.m_covariance_type = (covariance_type_t) type;
	}

	void py_ok_params_t :: set_ranges(double r1, double r2, double r3)
	{
		m_ok_param.set_ranges(r1, r2, r3);
	}

	void py_ok_params_t :: set_angles(double a1, double a2, double a3)
	{
		m_ok_param.set_angles(a1, a2, a3);
	}

	void py_ok_params_t :: set_sill(double sill)
	{
		m_ok_param.m_sill = sill;
	}

	void py_ok_params_t :: set_nugget(double nugget)
	{
		m_ok_param.m_nugget = nugget;
	}

	void py_ok_params_t :: set_radiuses(size_t r1, size_t r2, size_t r3)
	{
		m_ok_param.set_radiuses(r1, r2, r3);
	}

	void py_ok_params_t :: set_max_neighbours(size_t mn)
	{
		m_ok_param.m_max_neighbours = mn;
	}
}
