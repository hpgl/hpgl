/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V
#define MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V

#include "lvm_data.h"

namespace hpgl
{
	class no_mean_t
	{
	public:
		/*inline double mean(node_index_t)const
		{
			return 0;
		}*/		
		inline double operator[](node_index_t index)const
		{
			return 0;
		}
	};

	class single_mean_t
	{
		double m_mean;
	public:
		single_mean_t()
			: m_mean(0.0)
		{}

		single_mean_t(double mean)
			: m_mean(mean)
		{}

		/*inline double mean(node_index_t)const
		{
			return m_mean;
		}*/
		inline double operator[](node_index_t index)const
		{
			return m_mean;
		}
	};

	void create_means(const std::vector<indicator_probability_t> & marginal_probs, std::vector<boost::shared_ptr<single_mean_t> > & means);
	
}

#endif //MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V
