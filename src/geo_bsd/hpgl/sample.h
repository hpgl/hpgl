#ifndef __SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB
#define __SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB

#include "bs_random_generator.h"
#include "typedefs.h"
#include "gaussian_distribution.h"

namespace hpgl
{
	indicator_index_t
		sample(mt_random_generator_t & gen, const std::vector<indicator_probability_t> & probs);

	template <typename Cdf>
	double sample(mt_random_generator_t & gen, const Cdf& f) {
    	return f.inverse(gen());    
	}

	inline double sample(mt_random_generator_t & gen, 
			bool use_mean_and_variance, 
			cont_value_t mean, kriging_variance_t var)
	{
		if (use_mean_and_variance)
			return sample(gen, gaussian_cdf_t(mean, var));
		else
			return sample(gen, gaussian_cdf_t());
	}
}
#endif //__SAMPLE_H__F14739AA_FF94_421E_9E5B_6DF7AE2F8BDB
