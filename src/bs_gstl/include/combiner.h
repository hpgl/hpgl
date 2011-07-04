/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef COMBINER_H_INCLUDED_ASDLJASLKDJASDLJAKSJDLAKSJDKA345KL09I8UYHG56YHJM
#define COMBINER_H_INCLUDED_ASDLJASLKDJASDLJAKSJDLAKSJDKA345KL09I8UYHG56YHJM

#include "select.h"

namespace hpgl
{

	// computes value in point (center_node) from known neighbours (indices) and their weights (weights)	
	template<typename mean_provider_t>
	inline cont_value_t cont_combine(
		const property_array_t<cont_value_t> & property,
		const mean_provider_t & mp,
		const std::vector<node_index_t> & indices,
		const std::vector<kriging_weight_t> & weights,
		node_index_t center_node)
	{
		cont_value_t result = mp[center_node];
		for (size_t i = 0, end_i = indices.size(); i < end_i; ++i)
		{
			node_index_t node = indices[i];
			result += weights[i] * (property.get_at(node) - mp[node]);
		}
		return result;
	}

	template<typename value_t, typename result_t> 
	inline result_t combine(
		const std::vector<value_t> & values,
		const std::vector<kriging_weight_t> & weights,
		const std::vector<mean_t> & means,
		mean_t center_mean)
	{
		assert(values.size() == weights.size());
		assert(values.size() == means.size());
		result_t result = center_mean;
		for (size_t i = 0, end_i = values.size(); i < end_i; ++i)
		{			
			result += static_cast<result_t>(weights[i] * (values[i] - means[i]));
		}
		return result;
	}

	template<typename result_t, typename data_t, typename means_t> 
	inline void combine(
		const data_t & data,
		const means_t & mp,
		const std::vector<kriging_weight_t> & weights,
		node_index_t center_index,
		const std::vector<node_index_t> & indices,
		result_t & result)
	{
		typedef typename data_t::value_type value_t;
		std::vector<value_t> values;
		std::vector<mean_t> means;
		select(data, indices, values);
		select(mp, indices, means);
		result = combine<value_t, result_t>(values, weights, means, mp[center_index]);
	}

	inline cont_value_t cont_combine(		
		const std::vector<cont_value_t> & values,
		const std::vector<kriging_weight_t> & weights,
		const std::vector<mean_t> & means,
		mean_t center_mean)
	{		
		return combine<cont_value_t, cont_value_t>(values, weights, means, center_mean);
	}		


}

#endif //COMBINER_H_INCLUDED_ASDLJASLKDJASDLJAKSJDLAKSJDKA345KL09I8UYHG56YHJM
