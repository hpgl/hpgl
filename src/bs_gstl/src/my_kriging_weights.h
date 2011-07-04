/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
#define __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003

#include <sugarbox_grid.h>
#include <property_array.h>
#include <typedefs.h>


namespace hpgl
{
	class sk_constraints_t{};
	class ok_constraints_t{};
	class corellogram_constraints_t{};

	inline sk_constraints_t sk_constraints() {return sk_constraints_t();}
	inline ok_constraints_t ok_constraints() {return ok_constraints_t();}
	inline corellogram_constraints_t corellogram_constraints() {return corellogram_constraints_t();}

	template<typename MatrixLibrary, typename covariances_t, bool calc_variance, typename coord_t>
	bool sk_kriging_weights_2(						
			coord_t center_coord,			
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
		typename MatrixLib::Symmetric_matrix A;
		typename MatrixLib::Vector b;
		typename MatrixLib::Vector b2;

		A.resize(coords.size(), coords.size());
		b.resize(coords.size());
		b2.resize(coords.size());
		weights.resize(coords.size());

		if (coords.size() <= 0)
			return false;
		
		//build invariant
		for (int i = 1, end_i = coords.size(); i <= end_i; ++i)
		{
			for (int j = i, end_j = end_i; j <= end_j; ++j)
			{
				A(i, j) = covariances(coords[i-1], coords[j-1]);
			}
			b(i) = covariances(coords[i-1], center_coord);
			b2(i) = b(i);
		}

		bool system_solved = ( MatrixLib::solve(A, b, weights.begin()) == 0 );

		if (calc_variance)
		{
			if (system_solved)
			{		
			
				sugarbox_location_t center_loc;
				double cr0 = covariances(center_coord, center_coord);				
				variance = cr0;
				for (int i = 1, end_i = coords.size(); i <= end_i; ++i)
				{
					variance -= weights[i-1] * b2(i);
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
		return sk_kriging_weights_2<GSTL_TNT_lib, covariances_t, false, coord_t>(center, coords, covariancer, weights, variance);
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
		return sk_kriging_weights_2<GSTL_TNT_lib, covariances_t, true, coord_t>(center, coords, covariancer, weights, variance);
	}


	template<typename MatrixLibrary, typename covariances_t, bool calc_variance, typename coord_t>
	bool ok_kriging_weights_2(
			coord_t center,
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		typedef matrix_lib_traits<MatrixLibrary> MatrixLib;
		typename MatrixLib::Symmetric_matrix A;
		typename MatrixLib::Vector b;
		typename MatrixLib::Vector b2;

		int size = coords.size()+1;

		A.resize(size, size);
		b.resize(size);
		b2.resize(size);
		weights.resize(size);

		if (coords.size() <= 0)
			return false;

		for (int i = 1; i <= size - 1; ++i)
		{
			for (int j = i; j <= size - 1; ++j)
			{
				A(i, j) = covariances(
						coords[i-1], 
						coords[j-1]);
			}
			b(i) = covariances(
						coords[i-1], 
						center);
			b2(i) = b(i);
		}

		for (int j = 1; j < size; ++j)
		{
			A(size, j) = 1;
		}		
		A(size, size) = 0;
		b(size) = 1;
		
		bool result = MatrixLib::solve(A, b, weights.begin()) == 0;		
		if (result)
		{
			if (calc_variance)
			{
				sugarbox_location_t center_loc;
				double cr0 = covariances(center, center);
				variance = cr0;
				for (int i = 1, end_i = coords.size(); i <= end_i; ++i)
				{
					variance -= weights[i-1] * b2(i);
				}
				variance -= weights[coords.size()];
			}
		}
		weights.resize(coords.size());
		return result;
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
		return ok_kriging_weights_2<GSTL_TNT_lib, covariances_t, false, coord_t>(center, coords, covariances, weights, variance);
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
		return ok_kriging_weights_2<GSTL_TNT_lib, covariances_t, true, coord_t>(center, coords, covariances, weights, variance);
	}

	template<typename covariances_t, typename coord_t>
	bool corellogramed_weights_2(
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

		typedef matrix_lib_traits<GSTL_TNT_lib> MatrixLib;
		typename MatrixLib::Symmetric_matrix A;
		typename MatrixLib::Vector b;
		
		A.resize(size, size);
		b.resize(size);
		weights.resize(size);		

		double meanc = center_mean;
		double sigmac = sqrt(meanc * (1 - meanc));
		
		MatrixLib::Vector sigmas;
		sigmas.resize(size);

		for (int i = 1; i <= size; ++i)
		{			
			double meani = means[i-1];
			sigmas(i) = sqrt(meani * (1-meani));
		}

		//build invariant
		for (int i = 1; i <= size; ++i)
		{			
			for (int j = i; j <= size; ++j)
			{				
				A(i, j) = cov(
								coords[i-1], 
								coords[j-1]) 
						/ (sigmas(i) * sigmas(j));
			}
			b(i) = cov(coords[i-1], center) / (sigmas(i) * sigmac);
		}

		bool result = MatrixLib::solve(A, b, weights.begin()) == 0;
		for (int i = 1; i <= size; ++i)
		{
			weights[i-1] *= sigmac / sigmas(i);
		}
		return result;
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
		return sk_kriging_weights_2<GSTL_TNT_lib, covariances_t, false, coord_t>(
			center, 
			coords, 
			covariances, 
			weights, variance);
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
		return corellogramed_weights_2(center, center_mean, coords, covariances, means, weights);
	}	
}

#endif //__MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
