/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __KRIGING_H__69012DAB_4BA5_44E4_9DF9_749A5519E0AD
#define __KRIGING_H__69012DAB_4BA5_44E4_9DF9_749A5519E0AD

namespace hpgl
{

	/**/

	template<		
		typename values_t,
		typename defineds_t,
		typename coord_t,
		typename means_t,
		typename covariances_t,
		typename neighbourhood_lookup_t,
		typename weight_calculator_t,
		typename result_t>
	void kriging(
		std::vector<values_t*> values,
		std::vector<defineds_t*> defindes,
		const std::vector<params_t> & params,
		const reducer_t & reducer,
		const undefined_handler
		)
	{
		size_t size = input_property.size();

		#pragma omp parallel
		{
			#pragma omp for
			for (node_index_t node_idx = 0;	node_idx < size; ++node_idx)
			{
				struct result_t
				{
					double value;
					bool defined;					
				}

				std::vector<result_t> result;

				std::vector<indicator_probability_t> probs;
				for (int idx = 0; idx < params.m_category_count; ++idx)
				{
					indicator_probability_t prob;

					ki_result_t ki_result = kriging_interpolation(
						*params[idx].input_values,
						*params[idx].defineds,
						node_idx, grid[node_idx], *covariances[idx], 
						*mps[idx], nblookups[idx], weight_calculator(sk_constraints, input_property), prob);

					if (ki_result != KI_SUCCESS)
					{
						result_t res = {*mps[idx][node_idx], false};
						result.push_back(res)						
					}
					else
					{
						result_t res = {prob, true};
						result.push_back_t(res);
					}					
				}	

				output_poperty.set_at(node_idx, reducer(result));				
				#pragma omp critical
				{
					report.next_lap();
				}
			}
		}
		report.stop();	
	}

}

#endif //__KRIGING_H__69012DAB_4BA5_44E4_9DF9_749A5519E0AD