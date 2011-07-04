/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
#define __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003

#include <sugarbox_grid.h>
#include <property_array.h>
#include <typedefs.h>
#include "gauss_solver.h"


namespace hpgl
{
	class sk_constraints_t{};
	class ok_constraints_t{};
	class corellogram_constraints_t{};

	inline sk_constraints_t sk_constraints() {return sk_constraints_t();}
	inline ok_constraints_t ok_constraints() {return ok_constraints_t();}
	inline corellogram_constraints_t corellogram_constraints() {return corellogram_constraints_t();}

	template<typename covariances_t, bool calc_variance, typename coord_t>
	bool sk_kriging_weights_3(						
			coord_t center_coord,			
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		int size = coords.size();
		std::vector<double> A(coords.size() * coords.size());
		std::vector<double> b(coords.size());
		std::vector<double> b2(coords.size());
		weights.resize(coords.size());

		if (coords.size() <= 0)
			return false;
		
		//build invariant
		for (int i = 0, end_i = coords.size(); i < end_i; ++i)
		{
			for (int j = i, end_j = end_i; j < end_j; ++j)
			{
				A[i*size + j] = covariances(coords[i], coords[j]);
				A[j*size + i] = A[i*size + j];
			}
			b[i] = covariances(coords[i], center_coord);
			b2[i] = b[i];
		}

		bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);		

		if (calc_variance)
		{
			if (system_solved)
			{		
			
				sugarbox_location_t center_loc;
				double cr0 = covariances(center_coord, center_coord);				
				variance = cr0;
				for (int i = 0, end_i = coords.size(); i < end_i; ++i)
				{
					variance -= weights[i] * b2[i];
				}
			}
			else
			{
				variance = -1;
			}
		}	
		return system_solved;
	}

	template<typename covariances_t, typename coord_t>
	bool kriging_weights_2(
		const coord_t & center,
		const std::vector<coord_t> & coords,
		const covariances_t & covariancer,
		sk_constraints_t (*f)(void),
		std::vector<kriging_weight_t> & weights
		)
	{
		double variance;
		return sk_kriging_weights_3<covariances_t, false, coord_t>(center, coords, covariancer, weights, variance);
	}

	template<typename covariances_t, typename coord_t>
	bool kriging_weights_2(
		const coord_t & center,
		const std::vector<coord_t> & coords,
		const covariances_t & covariancer,
		sk_constraints_t (*f)(void),
		std::vector<kriging_weight_t> & weights,
		double & variance
		)
	{		
		return sk_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariancer, weights, variance);
	}
	
	template<typename covariances_t, bool calc_variance, typename coord_t>
	bool ok_kriging_weights_3(
			coord_t center,
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		if (coords.size() <= 0)
			return false;

		int size = coords.size()+1;

		std::vector<double> A(size*size);
		std::vector<double> b(size);
		std::vector<double> b2(size);
		weights.resize(size);
		
		for (int i = 0; i < size - 1; ++i)
		{
			for (int j = i; j < size - 1; ++j)
			{
				A[i*size + j] = covariances(coords[i], coords[j]);
				A[j*size + i] = A[i * size + j];
			}
			b[i] = covariances(coords[i], center);
			b2[i] = b[i];
		}

		for (int j = 0; j < size - 1; ++j)
		{
			A[(size-1) * size + j] = 1;
			A[j * size + (size - 1)] = 1;			
		}		
		A[size*size - 1] = 0;
		b[size-1] = 1;

		bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);		

		if (calc_variance)
		{
			if (system_solved)
			{
				sugarbox_location_t center_loc;
				double cr0 = covariances(center, center);
				variance = cr0;
				for (int i = 0, end_i = coords.size(); i < end_i; ++i)
				{
					variance -= weights[i] * b2[i];
				}
				variance -= weights[size - 1];
			}
			else
			{
				variance = -1;
			}
		}
		weights.resize(coords.size());
		return system_solved;
	}

	template<typename covariances_t, typename coord_t>
	bool kriging_weights_2(
			const coord_t & center,			
			const std::vector<coord_t> & coords,
			const covariances_t & covariances,
			ok_constraints_t (*f)(void),
			std::vector<kriging_weight_t> & weights
		)
	{
		double variance;
		return ok_kriging_weights_3<covariances_t, false, coord_t>(
				center, coords, covariances, weights, variance);		
	}


	template<typename covariances_t, typename coord_t>
	bool kriging_weights_2(
			const coord_t & center,			
			const std::vector<coord_t> & coords,
			const covariances_t & covariances,
			ok_constraints_t (*f)(void),
			std::vector<kriging_weight_t> & weights,
			double & variance
		)
	{		
		return ok_kriging_weights_3<covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
	}

	template<typename covariances_t, typename coord_t>
	bool corellogramed_weights_3(
			coord_t center,			
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const covariances_t & cov,
			const std::vector<mean_t> & means,
			std::vector<kriging_weight_t> & weights
			)
	{
		size_t size = coords.size();
		if (size <= 0)
			return false;

		std::vector<double> A(size * size);
		std::vector<double> b(size);
		weights.resize(size);		

		double meanc = center_mean;
		double sigmac = sqrt(meanc * (1 - meanc));
		
		std::vector<double> sigmas(size);		

		for (int i = 0; i < size; ++i)
		{			
			double meani = means[i];
			sigmas[i] = sqrt(meani * (1-meani));
		}

		//build invariant
		for (int i = 0; i < size; ++i)
		{			
			for (int j = 0; j < size; ++j)
			{				
				A[i * size + j] = 
					cov(coords[i], coords[j]) / (sigmas[i] * sigmas[j]);
			}
			b[i] = cov(coords[i], center) / (sigmas[i] * sigmac);
		}

		bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);		
		for (int i = 0; i < size; ++i)
		{
			weights[i] *= sigmac / sigmas[i];
		}
		return system_solved;
	}

	template<typename covariances_t, typename coord_t>
	bool indicator_weights_2(
		const coord_t & center,
		mean_t center_mean,
		const std::vector<coord_t> & coords,
		const std::vector<mean_t> & means,
		const covariances_t & covariances,
		sk_constraints_t (*f)(void),
		std::vector<kriging_weight_t> & weights)
	{
		double variance;
		return sk_kriging_weights_3<covariances_t, false, coord_t>
			(center, coords, covariances, weights, variance);		
	}

	template<typename covariances_t, typename coord_t>
	bool indicator_weights_2(
		const coord_t & center,
		mean_t center_mean,
		const std::vector<coord_t> & coords,
		const std::vector<mean_t> & means,
		const covariances_t & covariances,
		corellogram_constraints_t (*f)(void),
		std::vector<kriging_weight_t> & weights)
	{		
		return corellogramed_weights_3(center, center_mean, coords, covariances, means, weights);
	}


}

#endif //__MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
