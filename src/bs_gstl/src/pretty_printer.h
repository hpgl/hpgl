/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4
#define __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4

#include "typedefs.h"

namespace hpgl
{
	class ok_params_t;
	class sk_params_t;
	class ik_params_t;
	class sgs_params_t;
	class neighbourhood_param_t;
	class covariance_param_t;
	struct indicator_params_t;

	template<typename T>
	void print_param(const std::string & param, const T & value)
	{
		std::cout << param << ": " << value << "\n";
	}

	void print_algo_name(const std::string & name);

	void print_params(const neighbourhood_param_t & p);

	void print_params(const covariance_param_t & p);
	
	void print_params(const ok_params_t & p);

	void print_params(const sk_params_t & p);

	void print_params(const sgs_params_t & p);

	void print_params(const ik_params_t & p);

	void print_params(const indicator_params_t * params, int param_count, const mean_t * marginal_probs = 0);
}

#endif //__PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4
