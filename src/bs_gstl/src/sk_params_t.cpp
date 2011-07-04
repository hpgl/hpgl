/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include <sk_params.h>

namespace hpgl
{
	sk_params_t::sk_params_t()
		:m_mean(0),
		m_calculate_mean(true)
	{
	}



	std::ostream & operator <<(std::ostream & out, const sk_params_t& param)
	{
		out << (const ok_params_t&)param;

		if (param.m_calculate_mean)
			out << "\tMean: automatic\n";
		else
			out << "\tMean: " << param.mean() << "\n";

		return out;
	}
}

