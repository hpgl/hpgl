/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "typedefs.h"
#include "property_array.h"
#include "hpgl_exception.h"
#include "sugarbox_grid.h"
#include "sugarbox_neighbour_lookup.h"
#include "covariance_param.h"
#include "covariance_utils.h"
#include "is_informed_predicate.h"
#include "select.h"
#include "pretty_printer.h"
#include "progress_reporter.h"

namespace hpgl
{

template<typename coord_t, typename MatrixLib, typename primary_cov_model_t, typename cross_cov_model_t>
void build_system(
	const MatrixLib &,
	const coord_t & center, 
	const std::vector<coord_t> & coords,
	const primary_cov_model_t & primary_cov,
	const cross_cov_model_t & cross_cov,
	double secondary_variance,
	typename MatrixLib::Symmetric_matrix & A, 
	typename MatrixLib::Vector & b)
{
	int neighbour_count = coords.size();
	int matrix_size = neighbour_count + 1;

	A.resize(matrix_size, matrix_size);

	b.resize(matrix_size);

	for (int i = 0; i < neighbour_count; ++i)
	{
		for (int j = i; j < neighbour_count; ++j)
		{
			A(i+1, j+1) = primary_cov(coords[i], coords[j]);
		}
	}
	
	for (int i = 0; i < neighbour_count; ++i)
	{
		A(neighbour_count + 1, i + 1) = cross_cov(center, coords[i]);
		//A(i + 1, neighbour_count + 1) = cross_cov(coords[i], center);
		b(i + 1) = primary_cov(coords[i], center);
	}

	b(neighbour_count + 1) = cross_cov(coord_t(0,0,0), coord_t(0,0,0));

	A(neighbour_count + 1, neighbour_count + 1) = secondary_variance;
}

template<typename MatrixLib>
bool solve_system(const MatrixLib &, typename MatrixLib::Symmetric_matrix & A, typename MatrixLib::Vector & b, std::vector<kriging_weight_t> & weights)
{
	weights.resize(b.size());
	return MatrixLib::solve(A, b, weights.begin()) == 0;
}

template<typename coord_t, typename primary_cov_model_t, typename cross_cov_model_t, typename MatrixLib>
bool calc_weights(
		const MatrixLib & mlib,
		const coord_t & center,
		std::vector<coord_t> & coords,
		const primary_cov_model_t & primary_cov,
		const cross_cov_model_t & cross_cov,
		double secondary_variance,
		std::vector<kriging_weight_t> & weights
		)
{
	typename MatrixLib::Symmetric_matrix A; 
	typename MatrixLib::Vector b;
	
	// build system
	build_system(mlib, center, coords, primary_cov, cross_cov, secondary_variance, A, b);
	// solve systemk
	return solve_system(mlib, A, b, weights);
}

bool combine(
		const std::vector<cont_value_t> & values,
		const std::vector<kriging_weight_t> & weights,
		mean_t primary_mean,
		cont_value_t secondary_value,
		mean_t secondary_mean,
		cont_value_t & result
	    )
{
	if (values.size() != weights.size() - 1)
	{
		throw hpgl_exception("combine", boost::format( "values.size() = %s, weights.size() = %s. weights.size() - values.size() != 1") % values.size() % weights.size());
	}

	int size = values.size();
	result = primary_mean;
	for (int i = 0; i < size; ++i)
	{
		result += (values[i] - primary_mean) * weights[i];
	}
	result += weights[size] * (secondary_value - secondary_mean);
	return true;
}

template<typename data_t, typename primary_cov_model_t, typename cross_cov_model_t, typename n_lookup_t>
bool calc_value(
		node_index_t node,
		const data_t & primary_data,
		cont_value_t secondary_value,
		mean_t primary_mean,
		mean_t secondary_mean,
		double secondary_variance,
		const primary_cov_model_t & primary_cov,
	       	const cross_cov_model_t & cross_cov,
		const n_lookup_t & n_lookup,
		cont_value_t & result)
{
	typedef typename n_lookup_t::coord_t coord_t;
	std::vector<coord_t> coords;
	std::vector<node_index_t> indices;
	coord_t  center;
	
	n_lookup.find(node, is_informed_predicate_t<data_t>(primary_data), center, indices, coords);

	if (indices.size() <= 0)
		return false;

	std::vector<kriging_weight_t> weights;
	if (!calc_weights(matrix_lib_traits<GSTL_TNT_lib>(), center, coords, primary_cov, cross_cov, secondary_variance, weights))
		return false;

	std::vector<cont_value_t> values;
	select(primary_data, indices, values);
	
	if (!combine(values, weights, primary_mean, secondary_value, secondary_mean, result))
		return false;

	
	return true;
}

template<typename cov_model_t>
class cross_cov_model_mark_i_t
{
	double m_coef;
	cov_model_t * m_cov_model;
public:
	typedef typename cov_model_t::first_argument_type first_argument_type;
	typedef typename cov_model_t::second_argument_type second_argument_type;
	typedef typename cov_model_t::result_type result_type;

	cross_cov_model_mark_i_t(double p12, double d2, cov_model_t * cov_model)
		: m_cov_model(cov_model)
	{
		m_coef = p12 * sqrt(d2) / sqrt((*cov_model)(first_argument_type(0,0,0), second_argument_type(0,0,0)));
	}

	result_type operator()(const first_argument_type & p1, const second_argument_type & p2)const
	{
		return m_coef * (*m_cov_model)(p1, p2);
	}	
};

template<typename primary_cov_model_t, typename secondary_cov_model_t>
class cross_cov_model_mark_ii_t
{
	double m_coef;	
	secondary_cov_model_t * m_secondary_cov_model;
public:
	typedef typename secondary_cov_model_t::first_argument_type first_argument_type;
	typedef typename secondary_cov_model_t::second_argument_type second_argument_type;
	typedef typename secondary_cov_model_t::result_type result_type;

	cross_cov_model_mark_ii_t(double p12, primary_cov_model_t * primary_cov_model, secondary_cov_model_t * secondary_cov_model)
		: m_secondary_cov_model(secondary_cov_model)
	{
		double primary_variance = (*primary_cov_model)(first_argument_type(0,0,0), second_argument_type(0,0,0));
		double secondary_variance = (*secondary_cov_model)(first_argument_type(0,0,0), second_argument_type(0,0,0));
		m_coef = p12 * sqrt( secondary_variance / primary_variance );
	}

	result_type operator()(const first_argument_type & p1, const second_argument_type & p2)const
	{
		return m_coef * (*m_secondary_cov_model)(p1, p2);
	}
	
};

void simple_cokriging_markI(
		const sugarbox_grid_t & grid,
		const cont_property_array_t & input_prop, 
		const cont_property_array_t & secondary_data,
		mean_t primary_mean,
		mean_t secondary_mean,
		double secondary_variance,
		double correlation_coef,
		const neighbourhood_param_t & neighbourhood_params,
		const covariance_param_t & primary_cov_params,
		cont_property_array_t & output_prop)
{
	if (input_prop.size() != output_prop.size())
		throw hpgl_exception("simple_cokriging", boost::format("Input data size: %s. Output data size: %s. Must be equal.") % input_prop.size() % output_prop.size());

	print_algo_name("Simple Colocated Cokriging Markov Model I");
	print_params(neighbourhood_params);
	print_params(primary_cov_params);
	print_param("Primary mean", primary_mean);
	print_param("Secondary mean", secondary_mean);
	print_param("Secondary variance", secondary_variance);
	print_param("Correllation coef", correlation_coef);

	sugarbox_covariance_t cov;
	create_covariance(primary_cov_params, cov);

	cross_cov_model_mark_i_t<sugarbox_covariance_t> cross_cov(correlation_coef, secondary_variance, &cov);

	int data_size = input_prop.size();
	
	neighbour_lookup_t<sugarbox_grid_t, sugarbox_covariance_t> n_lookup(&grid, &cov, neighbourhood_params);

	progress_reporter_t report(data_size);

	report.start(data_size);

	// for each node
	for (node_index_t i = 0; i < data_size; ++i)
	{
	// 		calc value
		cont_value_t result = -500;		
		if (input_prop.is_informed(i))
		{			
			result = input_prop[i];
		}
		else
		{
			cont_value_t secondary_value = secondary_data.is_informed(i) ? secondary_data[i] : secondary_mean;
			if (!calc_value(i, input_prop, secondary_value, primary_mean, secondary_mean,
				    secondary_variance, cov, cross_cov, n_lookup, result))
			{
				result = primary_mean + secondary_value - secondary_mean;				
			}			
		}		
	// 		set value at node				
		output_prop.set_at(i, result);
		report.next_lap();
	}
}

void simple_cokriging_markII(
		const sugarbox_grid_t & grid,
		const cont_property_array_t & input_prop, 
		const cont_property_array_t & secondary_data,
		mean_t primary_mean,
		mean_t secondary_mean,		
		double correlation_coef,
		const neighbourhood_param_t & neighbourhood_params,
		const covariance_param_t & primary_cov_params,
		const covariance_param_t & secondary_cov_params,
		cont_property_array_t & output_prop)
{
	if (input_prop.size() != output_prop.size())
		throw hpgl_exception("simple_cokriging", boost::format("Input data size: %s. Output data size: %s. Must be equal.") % input_prop.size() % output_prop.size());

	print_algo_name("Simple Colocated Cokriging Markov Model II");
	print_params(neighbourhood_params);
	std::cout << "Primary covariation model:\n";
	print_params(primary_cov_params);
	std::cout << "Secondary covariation model:\n";
	print_params(secondary_cov_params);
	print_param("Primary mean", primary_mean);
	print_param("Secondary mean", secondary_mean);	
	print_param("Correllation coef", correlation_coef);

	sugarbox_covariance_t primary_cov;
	create_covariance(primary_cov_params, primary_cov);

	sugarbox_covariance_t secondary_cov;
	create_covariance(secondary_cov_params, secondary_cov);
	double secondary_variance = secondary_cov(sugarbox_grid_t::coord_t(0,0,0), sugarbox_grid_t::coord_t(0,0,0));

	cross_cov_model_mark_ii_t<sugarbox_covariance_t, sugarbox_covariance_t> cross_cov(correlation_coef, &primary_cov, &secondary_cov);

	int data_size = input_prop.size();
	
	neighbour_lookup_t<sugarbox_grid_t, sugarbox_covariance_t> n_lookup(&grid, &primary_cov, neighbourhood_params);

	progress_reporter_t report(data_size);

	report.start(data_size);

	// for each node
	for (node_index_t i = 0; i < data_size; ++i)
	{
	// 		calc value
		cont_value_t result = -500;		
		if (input_prop.is_informed(i))
		{			
			result = input_prop[i];
		}
		else
		{
			cont_value_t secondary_value = secondary_data.is_informed(i) ? secondary_data[i] : secondary_mean;
			if (!calc_value(i, input_prop, secondary_value, primary_mean, secondary_mean,
				    secondary_variance, primary_cov, cross_cov, n_lookup, result))
			{
				result = primary_mean + secondary_value - secondary_mean;			
			}			
		}		
	// 		set value at node				
		output_prop.set_at(i, result);
		report.next_lap();
	}
}

}
