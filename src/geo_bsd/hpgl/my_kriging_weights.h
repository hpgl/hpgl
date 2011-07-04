#ifndef __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
#define __MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003

//#define HPGL_SOLVER
#define LAPACK_SOLVER

extern "C" {
	#include "f2c.h"
	#include "blaswrap.h"
	#include "clapack.h"
	#undef abs
	#undef max
	#undef min
};


#include <sugarbox_grid.h>
#include <property_array.h>
#include <typedefs.h>
#include "gauss_solver.h"
#include "logging.h"


namespace hpgl
{
	template<typename covariances_t, bool calc_variance, typename coord_t>
	bool sk_kriging_weights_3(						
			coord_t center_coord,			
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		HPGL_LOG_STRING("Sk weights");
		HPGL_LOG_NEIGHBOURS(center_coord, coords);
			
		int size = (int)coords.size();
		std::vector<double> A(coords.size() * coords.size());
		std::vector<double> b(coords.size());
		std::vector<double> b2(coords.size());
		weights.resize(coords.size());
		if (coords.size() <= 0)
		{
			HPGL_LOG_STRING("No neighbours.");
			return false;
		}
		
		//build invariant
		for (int i = 0, end_i = (int)coords.size(); i < end_i; ++i)
		{
			for (int j = i, end_j = end_i; j < end_j; ++j)
			{
				A[i*size + j] = covariances(coords[i], coords[j]);
				A[j*size + i] = A[i*size + j];
			}
			b[i] = covariances(coords[i], center_coord);
			b2[i] = b[i];
		}

		HPGL_LOG_SYSTEM(&A[0], &b[0], size);

#ifdef HPGL_SOLVER

		// std::cout << "HPGL SOLVER MATRIX SIZE: " << size << std::endl;

		// INTERNAL

		std::vector<double> A_U(size*size,0.0);
		std::vector<double> A_L(size*size,0.0);

		//bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);	
		bool system_solved = cholesky_decomposition(&A[0], &A_U[0], &A_L[0], size);
		cholesky_solve(&A_L[0], &A_U[0], &b[0], &weights[0], size);

		HPGL_LOG_SYSTEM_SOLUTION(system_solved, &weights[0], size);

#endif

#ifdef LAPACK_SOLVER

		// std::cout << "LAPACK SOLVER MATRIX SIZE: " << size << std::endl;

		// CLAPACK
		bool system_solved = false;

		integer info_dec = 100;
		integer info_solve = 100;
		integer size_lap = size;
		integer b_size = 1;
		char matrix_type = 'U';

		// Cholesky decomposition
		dpotrf_(&matrix_type, &size_lap, &A[0], &size_lap, &info_dec);

		// Solve

		for (size_t i = 0; i < size; i ++)
			weights[i] = b[i];

		dpotrs_(&matrix_type, &size_lap, &b_size, &A[0],  &size_lap, &weights[0], &size_lap, &info_solve );

		if (info_dec == 0 && info_solve == 0 ) system_solved = true;

		HPGL_LOG_SYSTEM_SOLUTION(system_solved, &weights[0], size);

#endif

		//bool system_solved = cholesky_old(&A[0], &b[0], &weights[0], size);	

		if (calc_variance)
		{
			if (system_solved)
			{		
			
				sugarbox_location_t center_loc;
				double cr0 = covariances(center_coord, center_coord);				
				variance = cr0;
				for (int i = 0, end_i = (int) coords.size(); i < end_i; ++i)
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
	
	template<typename covariances_t, bool calc_variance, typename coord_t>
	bool ok_kriging_weights_3(
			coord_t center,
			const std::vector<coord_t> & coords,
			const covariances_t & covariances, 
			std::vector<kriging_weight_t> & weights,
			double & variance)
	{
		HPGL_LOG_STRING("Ok weights.");
		if (coords.size() <= 0)
		{
			HPGL_LOG_STRING("No neighbours.");
			return false;
		}

		int size = (int)coords.size();
		std::vector<double> A(coords.size() * coords.size());
		std::vector<double> b(coords.size());
		std::vector<double> b2(coords.size());
		weights.resize(coords.size());

		if (coords.size() <= 0)
			return false;
		
		//build invariant
		for (int i = 0, end_i = (int)coords.size(); i < end_i; ++i)
		{
			for (int j = i, end_j = end_i; j < end_j; ++j)
			{
				A[i*size + j] = covariances(coords[i], coords[j]);
				A[j*size + i] = A[i*size + j];
			}
			b[i] = covariances(coords[i], center);
			b2[i] = b[i];
		}

		HPGL_LOG_SYSTEM(&A[0], &b[0], size);

		//bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);		

		std::vector<double> ones(size, 1);
		
		std::vector<double> ones_result(size, 1);
		std::vector<double> sk_weights(size);

#ifdef HPGL_SOLVER

		// INTERNAL
		std::vector<double> A_U(size*size,0.0);
		std::vector<double> A_L(size*size,0.0);
		
		bool system_solved = cholesky_decomposition(&A[0], &A_U[0], &A_L[0], size);

		cholesky_solve(&A_L[0], &A_U[0], &b[0], &sk_weights[0], size);	
		cholesky_solve(&A_L[0], &A_U[0], &ones[0], &ones_result[0], size);	
#endif

#ifdef LAPACK_SOLVER

		// CLAPACK
		bool system_solved = false;

		integer info_dec = 100;
		integer info_solve = 100;
		integer size_lap = size;
		integer b_size = 1;
		char matrix_type = 'U';

		// Cholesky decomposition
		dpotrf_(&matrix_type, &size_lap, &A[0], &size_lap, &info_dec);

		// Solve

		for (size_t i = 0; i < size; i ++)
		{
			sk_weights[i] = b[i];
			ones_result[i] = ones[i];
		}

		dpotrs_(&matrix_type, &size_lap, &b_size, &A[0],  &size_lap, &sk_weights[0], &size_lap, &info_solve );
		dpotrs_(&matrix_type, &size_lap, &b_size, &A[0],  &size_lap, &ones_result[0], &size_lap, &info_solve );

		if (info_dec == 0 && info_solve == 0 ) system_solved = true;

#endif

		double SumSK = 0;
		double SumOnes = 0;

		for(int k = 0; k < size; k++)
		{
			SumSK += sk_weights[k];
			SumOnes += ones_result[k];
		}

		double mu = (SumSK - 1) / SumOnes;

		for (int k = 0; k < size; k++)
		{
			weights[k] = sk_weights[k] - mu * ones_result[k];
		}

		HPGL_LOG_SYSTEM_SOLUTION(system_solved, &weights[0], size);

		if (calc_variance)
		{
			if (system_solved)
			{
				sugarbox_location_t center_loc;
				double cr0 = covariances(center, center);
				variance = cr0;
				for (int i = 0, end_i = (int) coords.size(); i < end_i; ++i)
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
	bool corellogramed_weights_3(
			coord_t center,			
			mean_t center_mean,
			const std::vector<coord_t> & coords,
			const covariances_t & cov,
			const std::vector<mean_t> & means,
			std::vector<kriging_weight_t> & weights
			)
	{
		int size = (int) coords.size();
		if (size <= 0)
			return false;

		std::vector<double> A(size * size);
		std::vector<double> b(size);
		weights.resize(size);		

		double meanc = center_mean;
		double delta = 0.00001;
		
		if(meanc == 0)
		{
			meanc += delta;
		}
		if(meanc == 1)
		{
			meanc -= delta;
		}

		double sigmac = sqrt(meanc * (1 - meanc));
		
		std::vector<double> sigmas(size);		

		for (int i = 0; i < size; ++i)
		{			
			double meani = means[i];
			
			if(meani == 0)
			{
				meani += delta;
			}
			if(meani == 1)
			{
				meani -= delta;
			}

			sigmas[i] = sqrt(meani * (1-meani));
		}

	
		//build invariant
		for (int i = 0; i < size; ++i)
		{			
			for (int j = 0; j < size; ++j)
			{				
				A[i * size + j] = 
					cov(coords[i], coords[j]) * (sigmas[i] * sigmas[j]);
			}
			b[i] = cov(coords[i], center) * (sigmas[i] * sigmac);
		}

#ifdef HPGL_SOLVER

		// INTERNAL
		std::vector<double> A_U(size*size,0.0);
		std::vector<double> A_L(size*size,0.0);
		
		//bool system_solved = gauss_solve(&A[0], &b[0], &weights[0], size);	
		bool system_solved = cholesky_decomposition(&A[0], &A_U[0], &A_L[0], size);
		cholesky_solve(&A_L[0], &A_U[0], &b[0], &weights[0], size);

#endif

#ifdef LAPACK_SOLVER

		// CLAPACK
		bool system_solved = false;

		integer info_dec = 100;
		integer info_solve = 100;
		integer size_lap = size;
		integer b_size = 1;
		char matrix_type = 'U';

		// Cholesky decomposition
		dpotrf_(&matrix_type, &size_lap, &A[0], &size_lap, &info_dec);

		// Solve

		for (size_t i = 0; i < size; i ++)
			weights[i] = b[i];

		dpotrs_(&matrix_type, &size_lap, &b_size, &A[0],  &size_lap, &weights[0], &size_lap, &info_solve );

		if (info_dec == 0 && info_solve == 0 ) system_solved = true;

#endif

/*
		for (int i = 0; i < size; ++i)
		{
			weights[i] *= sigmac / sigmas[i];
		}
*/
		return system_solved;
	}

}

#endif //__MY_KRIGING_WEIGHTS_H__B6211BC7_74C1_4D96_AB05_286A62D0F003
