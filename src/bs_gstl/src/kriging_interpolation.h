/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872
#define __KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872

#include "select.h"

namespace hpgl
{
	

	template<typename constraints_t, typename value_t>
	class weight_calculator_t
	{};

	template<typename constraints_t>
	class weight_calculator_t<constraints_t, cont_value_t>
	{
		constraints_t m_constraints;
	public:
		weight_calculator_t(constraints_t constr)
			: m_constraints(constr)
		{}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(
			const coord_t & center,
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			return kriging_weights_2(center, coords, covariances, m_constraints, weights);
		}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return kriging_weights_2(center, coords, covariances, m_constraints, weights, variance);
		}			
	};

	template<typename constraints_t>
	class weight_calculator_t<constraints_t, indicator_value_t>
	{
		constraints_t m_constraints;
	public:
		weight_calculator_t(constraints_t constr)
			: m_constraints(constr)
		{}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t & means,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			return indicator_weights_2(center, center_mean, coords, means, covariances, m_constraints, weights);
		}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t & means,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return indicator_weights_2(center, center_mean, coords, means, covariances, m_constraints, weights, variance);
		}			
	};

	template<typename constraints_t, typename value_t>
	weight_calculator_t<constraints_t, value_t> weight_calculator(constraints_t constraints, const property_array_t<value_t> &)
	{
		return weight_calculator_t<constraints_t,value_t>(constraints);
	}

	enum ki_result_t
	{
		KI_SUCCESS = 0,
		KI_NO_NEIGHBOURS = 1,
		KI_SINGULARITY = 2
	};

	template<typename params_t, typename grid_t, typename result_t>
	ki_result_t kriging_interpolation(
		const params_t & params,		
		node_index_t index,		
		result_t & result)
	{
		return kriging_interpolation(
			*params.input_values, 
			*params.defineds, 
			index, 
			*params.covariances, 
			*params.means, 
			*params.neighbour_lookup, 
			*params.weight_calculator, result);
	}

	template<		
		typename values_t,
		typename defineds_t,
		typename means_t,
		typename covariances_t,
		typename neighbourhood_lookup_t,
		typename weight_calculator_t,
		typename result_t>
	ki_result_t kriging_interpolation(
			const values_t & input_values,
			const defineds_t & defineds,
			node_index_t index,			
			const covariances_t & cov,
			const means_t & mp,
			const neighbourhood_lookup_t & nl,
			const weight_calculator_t & wc,
			result_t & result
		)
	{
		typedef typename values_t::value_type value_t;
		typedef typename neighbourhood_lookup_t::coord_t coord_t;
		std::vector<node_index_t> indices;
		std::vector<kriging_weight_t> weights;
		std::vector<mean_t> means;
		std::vector<value_t> values;
		std::vector<coord_t> coords;
		coord_t node_coord;
		
		nl.find(index, defineds, node_coord, indices, coords);
		if (indices.size() <= 0)
			return KI_NO_NEIGHBOURS;

		select(mp, indices, means);
		bool success = wc(node_coord, mp[index], coords, means, cov, weights);		
		if (success)
		{			
			select(input_values, indices, values);					
			result = combine<value_t, result_t>(values, weights, means, mp[index]);

			return KI_SUCCESS;
		}
		else
		{			
			return KI_SINGULARITY;			
		}		
	}

	template<		
		typename values_t,
		typename defineds_t,
		typename means_t,
		typename covariances_t,
		typename neighbourhood_lookup_t,
		typename weight_calculator_t,
		typename result_t>
	ki_result_t kriging_interpolation(
			const values_t & input_values,
			const defineds_t & defineds,
			node_index_t index,			
			const covariances_t & cov,
			const means_t & mp,
			const neighbourhood_lookup_t & nl,
			const weight_calculator_t & wc,
			result_t & result,
			double & variance
		)
	{
		typedef typename values_t::value_type value_t;
		typedef typename neighbourhood_lookup_t::coord_t coord_t;
		std::vector<node_index_t> indices;
		std::vector<kriging_weight_t> weights;
		std::vector<mean_t> means;
		std::vector<value_t> values;
		std::vector<coord_t> coords;			
		coord_t node_coord;

		nl.find(index, defineds, node_coord, indices, coords);
		if (indices.size() <= 0)
			return KI_NO_NEIGHBOURS;

		select(mp, indices, means);
		bool success = wc(node_coord, mp[index], coords, means, cov, weights, variance);		

		if (success)
		{			
			select(input_values, indices, values);					
			result = combine<value_t, result_t>(values, weights, means, mp[index]);
			return KI_SUCCESS;
		}
		else
		{			
			return KI_SINGULARITY;			
		}		
	}

}

#endif //__KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872