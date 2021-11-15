#ifndef __KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872
#define __KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872

#include "select.h"
#include "combiner.h"
#include "my_kriging_weights.h"

namespace hpgl
{	
	class sk_weight_calculator_t
	{		
	public:
		template<typename covariances_t, typename coord_t>
		bool operator()(
			const coord_t & center,		
			const std::vector<coord_t> & coords,			
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			double variance;
			return sk_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(
			const coord_t & center,
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			double variance;
			return sk_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename covariances_t, typename coord_t>
		bool operator()(const coord_t & center, 			
			const std::vector<coord_t> & coords,
			
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return sk_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
		}	

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return sk_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
		}	

		template<typename covariances_t, typename coord_t>
		inline bool first_stage(
						 const coord_t & center, 
						 const std::vector<coord_t> & coords, 
						 const covariances_t & covariances,
						 std::vector<kriging_weight_t> & weights)const
		{	
			double variance;
			return sk_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}	

		template<typename means_t>
		inline bool second_stage(
						  std::vector<kriging_weight_t> & weights,
						  const mean_t center_mean,
						  const means_t & means)const
		{
			return true;
		}
	};
	
	class ok_weight_calculator_t
	{
	public:
		template<typename covariances_t, typename coord_t>
		bool operator()(
			const coord_t & center,		
			const std::vector<coord_t> & coords,		
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			double variance;
			return ok_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(
			const coord_t & center,
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			double variance;
			return ok_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename covariances_t, typename coord_t>
		bool operator()(const coord_t & center, 			
			const std::vector<coord_t> & coords,			
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return ok_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t &,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights, double & variance)const
		{
			return ok_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
		}			
	};

	
	class corellogram_weight_calculator_t
	{		
	public:
		template<typename covariances_t, typename means_t, typename coord_t>
		bool operator()(const coord_t & center, 
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const means_t & means,
			const covariances_t & covariances,
			std::vector<kriging_weight_t> & weights)const
		{
			return corellogramed_weights_3(center, center_mean, coords, covariances, means, weights);
		}

		template<typename covariances_t, typename coord_t>
		inline bool first_stage(
						 const coord_t & center, 
						 const std::vector<coord_t> & coords, 
						 const covariances_t & covariances,
						 std::vector<kriging_weight_t> & weights)const
		{
		    double variance;
			return sk_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
		}

		template<typename means_t>
		inline bool second_stage(
						  std::vector<kriging_weight_t> & weights,
						  const mean_t center_mean,
						  const means_t & means)const
		{
			int size = (int) weights.size();

			double delta = 0.00001;
			double meanc = center_mean;
			
			if(meanc == 0)
			{
				meanc += delta;
			}
			if(meanc == 1)
			{
				meanc -= delta;
			}

			double sigmac = sqrt(meanc * (1 - meanc));
			
			double meani = 0;
			for (int i = 0; i < size; ++i)
			{
				meani = means[i];
			
				if(meani == 0)
				{
					meani += delta;
				}
				if(meani == 1)
				{
					meani -= delta;
				}

				double sigma = sqrt(meani * (1 - meani));
				weights[i] /= sigmac / sigma;
			}
			return true;
		}
	};


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
			return ki_result_t::KI_NO_NEIGHBOURS;

		select(mp, indices, means);
		bool success = wc(node_coord, mp[index], coords, means, cov, weights);		
		if (success)
		{			
			select(input_values, indices, values);					
			result = combine<value_t, result_t>(values, weights, means, mp[index]);

			return ki_result_t::KI_SUCCESS;
		}
		else
		{			
			return ki_result_t::KI_SINGULARITY;
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
			return ki_result_t::KI_NO_NEIGHBOURS;

		select(mp, indices, means);
		bool success = wc(node_coord, mp[index], coords, means, cov, weights, variance);		

		if (success)
		{			
			select(input_values, indices, values);					
			result = combine<value_t, result_t>(values, weights, means, mp[index]);
			return ki_result_t::KI_SUCCESS;
		}
		else
		{			
			return ki_result_t::KI_SINGULARITY;
		}		
	}

}

#endif //__KRIGING_INTERPOLATION_H__31DBE328_667D_49C4_A4CB_830C3FA89872
