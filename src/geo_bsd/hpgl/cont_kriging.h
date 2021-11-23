#ifndef CONT_KRIGING_H_INCLUDED_LJLDFJVW450934VDV9ONV09NOASU92N34FOKLSDFGP3Q98SXNP
#define CONT_KRIGING_H_INCLUDED_LJLDFJVW450934VDV9ONV09NOASU92N34FOKLSDFGP3Q98SXNP

#include "combiner.h"
#include "covariance_field.h"
#include "progress_reporter.h"
#include "kriging_stats.h"
#include <omp.h>
#include "typedefs.h"
#include "select.h"
#include "precalculated_covariance.h"
#include "kriging_interpolation.h"
#include "neighbourhood_lookup.h"
#include "is_informed_predicate.h"
#include "cov_model.h"

namespace hpgl
{
	/*!
 	* Enumerationg specifing ways of handling kriging errors (absence of neighbours or singularity of matrix)
 	*/
	enum kriging_failure_handling
	{
		mean_on_failure, //!< Put the mean value
		undefined_on_failure //!< Leave node undefined
	};

	/*! 
 	*  Generic kriging alghorithm for continuous data. Uses OpenMP.
 	*  
 	*/
	template<
			typename grid_t, //!< Grid-With-Neighbour-Lookup concept
			typename data_t, //!< Property concept
			typename means_t, //!< Mean provider concept
			typename covariances_t, //!< Covariance Model Concept
			typename weight_calculator_t //!< Weight-Calculator concept. 
		>
	void cont_kriging(
			const data_t & input_property, //!< input data
			const grid_t & grid, 
			const neighbourhood_param_t /*ok_params_t*/ & params, //!< parameters of neighbourhood search
			const means_t & means, //!< mean values of data
			const covariances_t & cov, //!< covariance model
			const weight_calculator_t & wc, //!< Weight calculator specifies methods of calculating weights (OK, SK or LVM Kriging)		
			data_t & output_property, //!< resulting data
			progress_reporter_t & report, //!< object for tracking progress
			kriging_stats_t & stats, //!< returns some statistics of calculation
			kriging_failure_handling fh = mean_on_failure //!< Way of handling kriging errors (absence of neighbours, singularity).
		)
	{
		assert(input_property.size() == output_property.size());
		assert(grid.size() == input_property.size());
		
		double sum = 0;
		stats.m_points_calculated = 0;
		stats.m_points_without_neighbours = 0;
		stats.m_mean = 0;				
				
		typedef indexed_neighbour_lookup_t<grid_t, covariances_t> nl_t;
		nl_t neighbour_lookup(&grid, &cov, params);

		for (node_index_t node = 0; node < input_property.size(); ++node)
		{
			if (input_property.is_informed(node))
			{
				neighbour_lookup.add_node(node);
			}
		}

		report.start();
		node_index_t idx_end = input_property.size();
		unsigned long points_calculated = 0;
		unsigned long points_without_neighbours = 0;
#pragma omp parallel
{
		#pragma omp for reduction(+: points_calculated) reduction(+: points_without_neighbours) reduction(+: sum) 
		for(node_index_t idx = 0; idx < idx_end; ++idx)	
		{	
			if (!input_property.is_informed(idx))
			{				
				cont_value_t value;
				switch(kriging_interpolation(input_property, is_informed_predicate_t<data_t>(input_property), idx, cov, means, neighbour_lookup, wc, value))
				{
				case ki_result_t::KI_SUCCESS:
					output_property.set_at(idx, value);
					points_calculated++;				
					sum += value;
					break;
				case ki_result_t::KI_NO_NEIGHBOURS:
					points_without_neighbours++;				
					if (fh == kriging_failure_handling::mean_on_failure)
						output_property.set_at(idx, means[idx]);
						sum += means[idx];
					break;
				case ki_result_t::KI_SINGULARITY:
					if (fh == kriging_failure_handling::mean_on_failure)
						output_property.set_at(idx, means[idx]);
						sum += means[idx];
					break;
				}				
			}
			else
			{
				output_property.set_at(idx, input_property.get_at(idx));
			}

			#pragma omp critical
			{
				report.next_lap();	
			}
		}
}	

		report.stop();
		stats.m_points_calculated = points_calculated;
		stats.m_points_without_neighbours = points_without_neighbours;
		stats.m_mean = sum / output_property.size();
		stats.m_speed_nps = report.iterations_per_second();
		write(boost::format("Done. Average speed: %1% point/sec.\n") % report.iterations_per_second());
	}
 	
}
#endif //CONT_KRIGING_H_INCLUDED_LJLDFJVW450934VDV9ONV09NOASU92N34FOKLSDFGP3Q98SXNP
