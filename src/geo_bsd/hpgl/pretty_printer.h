#ifndef __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4
#define __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4

#include "typedefs.h"
#include "output.h"

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
		write(boost::format("%1%: %2%\n") % param % value);
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
