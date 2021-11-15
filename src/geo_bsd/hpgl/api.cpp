#include "stdafx.h"
#include <iostream>
#include <boost/format.hpp>

#include "api.h"
#include "api_helpers.hpp"
#include "hpgl_core.h"
#include "sugarbox_grid.h"
#include "ok_params.h"
#include "sk_params.h"
#include "sgs_params.h"
#include "ik_params.h"
#include "median_ik.h"
#include "property_writer.h"
#include "progress_reporter.h"
#include "hpgl_exception.h"
#include "output.h"

static void init_cov_param(hpgl::covariance_param_t * cp, hpgl_cov_params_t * params)
{
	for (int i = 0; i < 3; ++i)
	{
		cp->m_ranges[i] = params->m_ranges[i];
		cp->m_angles[i] = params->m_angles[i];
	}
	cp->m_sill = params->m_sill;
	cp->m_nugget = params->m_nugget;
	cp->m_covariance_type = (hpgl::covariance_type_t) params->m_covariance_type;	
}

static void 
handle_exception(const std::exception & ex)
{
	hpgl::set_last_exception_message(ex.what());
}

extern "C" {

HPGL_API char * hpgl_get_last_exception_message()
{
	return (char *) hpgl::get_last_exception_message().c_str();
}

HPGL_API void hpgl_set_thread_num(int n_threads)
{
	hpgl::set_thread_num(n_threads);
}

HPGL_API int hpgl_get_thread_num()
{
	return hpgl::get_thread_num();
}

HPGL_API void hpgl_read_inc_file_float(
		char * filename,
		float undefined_value,
		int size,
		float * data,
		unsigned char * mask)
{
	hpgl::read_inc_file_float(
			filename,
			undefined_value,
			size,
			data,
			mask);
}

HPGL_API void hpgl_read_inc_file_byte(
		char * filename,
		int undefined_value,
		int size,
		unsigned char * data,
		unsigned char * mask,
		unsigned char * values,
		int values_count)
{
	hpgl::read_inc_file_byte(
			filename,
			undefined_value,
			size,
			data,
			mask);

	for (int i = 0; i < size; ++i)
	{
		if (mask[i] != 0)
		{
			for (int j = 0; j < values_count; ++j)
			{
				if (data[i] == values[j])
				{
					data[i] = j;
					break;
				}
			}
		}
	}
}

HPGL_API void hpgl_write_inc_file_float(
		char * filename,
		hpgl_cont_masked_array_t * arr,
		float undefined_value,
		char * name)
{
	using namespace hpgl;
	property_writer_t writer;
	writer.init(filename, name);
	cont_property_array_t prop(
			arr->m_data, 
			arr->m_mask,
			get_shape_volume(&(arr->m_shape)));
	writer.write_double(prop, undefined_value);
}

void init_remap_table(unsigned char * values, int values_count, int indicator_count, std::vector<unsigned char> & remap_table)
{
	using namespace boost;
	if (values_count == indicator_count)
	{
		remap_table.assign(values, values+values_count);
	}
	else if (values_count > indicator_count)
	{
		LOGWARNING(format("Given %1% values for %2% indicators. Ignoring extra values.\n") % values_count % indicator_count);
		remap_table.assign(values, values + indicator_count);
	}
	else
	{
		if (values_count > 0)
			LOGWARNING(format("Warning: Given %1% values for %2% indicators. Ignoring values. Using [0, 1, 2 ..]\n") % values_count % indicator_count);
		for (int i = 0; i < indicator_count; ++i)
			remap_table.push_back(i);
	}
}

HPGL_API void hpgl_write_inc_file_byte(
		char * filename,
		hpgl_ind_masked_array_t * arr,
		int undefined_value,		
		char * name,
		unsigned char * values,
		int values_count)
{
	using namespace hpgl;
	std::vector<unsigned char> remap_table;
	init_remap_table(values, values_count, arr->m_indicator_count, remap_table);

	property_writer_t writer;
	writer.init(filename, name);
	indicator_property_array_t prop(
			arr->m_data, 
			arr->m_mask, 
			get_shape_volume(&(arr->m_shape)),
			arr->m_indicator_count);
	writer.write_byte(prop, undefined_value, remap_table);
}

HPGL_API void
hpgl_write_gslib_cont_property(
		hpgl_cont_masked_array_t * data,
		const char * filename,
		const char * name,
		double undefined_value)
{
	using namespace hpgl;
	int size = get_shape_volume(&data->m_shape);
	sp_double_property_array_t prop(new cont_property_array_t(data->m_data, data->m_mask, size));
	hpgl::property_writer_t writer;
	writer.init(filename, name);
	writer.write_gslib_double(prop, undefined_value);
}

HPGL_API void
hpgl_write_gslib_byte_property(
		hpgl_ind_masked_array_t * data,
		const char * filename,
		const char * name,
		double undefined_value,
		unsigned char * values,
		int values_count)
{
	using namespace hpgl;
	int size = get_shape_volume(&data->m_shape);
	sp_byte_property_array_t prop( new indicator_property_array_t(data->m_data, data->m_mask, size, data->m_indicator_count));
	std::vector<unsigned char> remap_table;
	init_remap_table(values, values_count, data->m_indicator_count, remap_table);

	property_writer_t writer;
	writer.init(filename, name);
	writer.write_gslib_byte(prop, (unsigned char)undefined_value, remap_table);
}

HPGL_API void hpgl_ordinary_kriging(
    hpgl_cont_masked_array_t * input_data,
    hpgl_ok_params_t * params,
    hpgl_cont_masked_array_t * output_data)
{
	using namespace hpgl;
	int in_size = get_shape_volume(&input_data->m_shape);
	int out_size = get_shape_volume(&output_data->m_shape);
		
	cont_property_array_t in_prop(input_data->m_data, input_data->m_mask, in_size);
	cont_property_array_t out_prop(output_data->m_data, output_data->m_mask, out_size);
	
	sugarbox_grid_t grid;
	init_grid(grid, &input_data->m_shape);

	ok_params_t ok_p;
	ok_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	ok_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	ok_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	ok_p.m_sill = params->m_sill;
	ok_p.m_nugget = params->m_nugget;
	
	ok_p.set_radiuses(
			params->m_radiuses[0],
			params->m_radiuses[1],
			params->m_radiuses[2]);

	ok_p.m_max_neighbours = params->m_max_neighbours;

	hpgl::ordinary_kriging(in_prop, grid, ok_p, out_prop, true);
}

static void init_sk_params(hpgl_sk_params_t * params, hpgl::sk_params_t & sk_p)
{
	using namespace hpgl;
	sk_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	sk_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	sk_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	sk_p.m_sill = params->m_sill;
	sk_p.m_nugget = params->m_nugget;
	
	sk_p.set_radiuses(
			params->m_radiuses[0],
			params->m_radiuses[1],
			params->m_radiuses[2]);

	sk_p.m_max_neighbours = params->m_max_neighbours;

	if (!params->m_automatic_mean)
	{
		sk_p.set_mean(params->m_mean);
	}
}

HPGL_API void hpgl_simple_kriging(   
    float * input_data,
    unsigned char * input_mask,
    hpgl_shape_t * input_data_shape,
    hpgl_sk_params_t * params,
    float * output_data,
    unsigned char * output_mask,
    hpgl_shape_t * output_data_shape)
{
	using namespace hpgl;
	int in_size = input_data_shape->m_data[0] * input_data_shape->m_data[1] * input_data_shape->m_data[2];
	int out_size = output_data_shape->m_data[0] * output_data_shape->m_data[1] * output_data_shape->m_data[2];
		
	cont_property_array_t in_prop(input_data, input_mask, in_size);
	cont_property_array_t out_prop(output_data, output_mask, out_size);
	
	sugarbox_grid_t grid;
	grid.init(
			input_data_shape->m_data[0],
			input_data_shape->m_data[1],
			input_data_shape->m_data[2]
			  );

	sk_params_t sk_p;
	init_sk_params(params, sk_p);

	hpgl::simple_kriging(in_prop, grid, sk_p, out_prop);
	}

}

HPGL_API int
hpgl_simple_kriging_weights(
		float * center_coords,
		float * neighbours_x,
		float * neighbours_y,
		float * neighbours_z,
		int neighbours_count,
		hpgl_cov_params_t * params,
		float * weights)
{
	using namespace hpgl;
	real_location_t center(center_coords[0], center_coords[1], center_coords[2]);
	
	std::vector<real_location_t> neighbour_coords(neighbours_count);
	for (int i = 0; i < neighbours_count; ++i)
	{
		neighbour_coords[i][0] = neighbours_x[i];
		neighbour_coords[i][1] = neighbours_y[i];
		neighbour_coords[i][2] = neighbours_z[i];
	}

	std::vector<kriging_weight_t> weights2;
	double variance;

	sk_params_t sk_p;

	sk_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	sk_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	sk_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	sk_p.m_sill = params->m_sill;
	sk_p.m_nugget = params->m_nugget;

	simple_kriging_weights(
			&sk_p,
			center,
			neighbour_coords,
			weights2,
			variance);

	for (int i = 0; i < neighbours_count; ++i)
	{
		weights[i] = weights2.at(i);
	}
	return 0;
}

HPGL_API void hpgl_lvm_kriging(
    float * input_data,
    unsigned char * input_mask,
    hpgl_shape_t * input_data_shape,
    float * mean_data,
    hpgl_shape_t * mean_data_shape,
    hpgl_ok_params_t * params,
    float * output_data,
    unsigned char * output_mask,
    hpgl_shape_t * output_data_shape)
{
	using namespace hpgl;
	int size = get_shape_volume(input_data_shape);
	cont_property_array_t input_prop(input_data, input_mask, size);
	sugarbox_grid_t grid;
	init_grid(grid, input_data_shape);

	ok_params_t ok_p;
	ok_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	ok_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	ok_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	ok_p.m_sill = params->m_sill;
	ok_p.m_nugget = params->m_nugget;
	
	ok_p.set_radiuses(
			params->m_radiuses[0],
			params->m_radiuses[1],
			params->m_radiuses[2]);

	ok_p.m_max_neighbours = params->m_max_neighbours;

	cont_property_array_t out_prop(output_data, output_mask, size);
	lvm_kriging(input_prop, mean_data, grid, ok_p, out_prop);
}

HPGL_API void
hpgl_indicator_kriging(
		hpgl_ind_masked_array_t * in_data,
		hpgl_ind_masked_array_t * out_data,
		hpgl_ik_params_t * params,
		int indicator_count)
{
	using namespace hpgl;
	int size = get_shape_volume(&in_data->m_shape);
	int size2 = get_shape_volume(&out_data->m_shape);
	assert (size == size2);
	indicator_property_array_t in_prop(in_data->m_data, in_data->m_mask, size, in_data->m_indicator_count);
	indicator_property_array_t out_prop(out_data->m_data, out_data->m_mask, size2, out_data->m_indicator_count);

	ik_params_t ikp;
	init_sis_params(params, indicator_count, &ikp);

	sugarbox_grid_t grid;
	init_grid(grid, &(in_data->m_shape));

	indicator_kriging(in_prop, grid, ikp, out_prop);
}

HPGL_API void 
hpgl_sgs_simulation(
		hpgl_cont_masked_array_t * data,
		hpgl_sgs_params_t * params,		
		hpgl_non_parametric_cdf_t * cdf,
		double * mean,
		hpgl_ubyte_array_t * simulation_mask)

{
	using namespace hpgl;
	int size = get_shape_volume(&(data->m_shape));
	cont_property_array_t prop(data->m_data, data->m_mask, size);

	sugarbox_grid_t grid;
	init_grid(grid, &(data->m_shape));

	sgs_params_t sgs_p;
	init_sgs_params(params, &sgs_p);
	
	if (mean != 0)
		sgs_p.set_mean(*mean);
	sgs_p.m_lvm = 0;
	sgs_p.m_mean_kind = mean != 0 ? e_mean_stationary : e_mean_stationary_auto;
	hpgl::sequential_gaussian_simulation(
			grid,
		    sgs_p,
			prop,
			cdf,
			simulation_mask != 0 ? simulation_mask->m_data : 0);	
}


HPGL_API void hpgl_sgs_lvm_simulation(
		hpgl_cont_masked_array_t * data,
		hpgl_sgs_params_t * params,
		hpgl_non_parametric_cdf_t * cdf,
		hpgl_float_array_t * means,
		hpgl_ubyte_array_t * simulation_mask)
{
	using namespace hpgl;
	int size = get_shape_volume(&(data->m_shape));
	cont_property_array_t prop(data->m_data, data->m_mask, size);

	sugarbox_grid_t grid;
	init_grid(grid, &(data->m_shape));

	sgs_params_t sgs_p;
	init_sgs_params(params, &sgs_p);

	sgs_p.m_lvm = means->m_data;
	sgs_p.m_mean_kind = e_mean_varying;
	
	hpgl::sequential_gaussian_simulation_lvm(
			grid,
			sgs_p,
			means->m_data,
			prop,
			cdf,
			simulation_mask != 0 ? simulation_mask->m_data : 0);
}

HPGL_API void hpgl_median_ik(
		hpgl_ind_masked_array_t * in_data,
		hpgl_median_ik_params_t * params,
		hpgl_ind_masked_array_t * out_data)
{
	using namespace hpgl;

	int size = get_shape_volume(&(in_data->m_shape));

	sugarbox_grid_t grid;
	init_grid(grid, &(in_data->m_shape));

	median_ik_params mik_p;
	mik_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	mik_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	mik_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	mik_p.m_sill = params->m_sill;
	mik_p.m_nugget = params->m_nugget;
	
	mik_p.set_radiuses(
			params->m_radiuses[0],
			params->m_radiuses[1],
			params->m_radiuses[2]);

	mik_p.m_max_neighbours = params->m_max_neighbours;
	mik_p.m_marginal_probs[0] = params->m_marginal_probs[0];
	mik_p.m_marginal_probs[1] = params->m_marginal_probs[1];

	indicator_property_array_t in_prop(
			in_data->m_data, 
			in_data->m_mask,
			size,
			2);
	indicator_property_array_t out_prop(
			out_data->m_data,
			out_data->m_mask,
			size,
			2);
	median_ik_for_two_indicators(mik_p, grid, in_prop, out_prop);
		
}

HPGL_API void
hpgl_sis_simulation(
		hpgl_ind_masked_array_t * data,
		hpgl_ik_params_t * params,
		int indicator_count,
		int seed,
		hpgl_ubyte_array_t * simulation_mask)
{
	using namespace hpgl;
	int size = get_shape_volume(&data->m_shape);
	indicator_property_array_t prop(data->m_data, data->m_mask, size, indicator_count);
	
	sugarbox_grid_t grid;
	init_grid(grid, &data->m_shape);

	ik_params_t ikp;
	init_sis_params(params, indicator_count, &ikp);

	progress_reporter_t rep(size);

	sequential_indicator_simulation(
			prop,
			grid,
			ikp,
			seed,
			rep,
			false,
			simulation_mask !=0 ? simulation_mask->m_data : 0);
}

HPGL_API void
hpgl_sis_simulation_lvm(
		hpgl_ind_masked_array_t * data,
		hpgl_ik_params_t * params,
		hpgl_float_array_t * mean_data,
		int indicator_count,
		int seed,
		hpgl_ubyte_array_t * simulation_mask,
		int use_correlograms)
{
	using namespace hpgl;
	int size = get_shape_volume(&data->m_shape);
	indicator_property_array_t prop(data->m_data, data->m_mask, size, indicator_count);
	
	sugarbox_grid_t grid;
	init_grid(grid, &data->m_shape);

	ik_params_t ikp;
	init_sis_params(params, indicator_count, &ikp);

	progress_reporter_t rep(size);
	
	std::vector<const mean_t *> means;
	for (int i = 0; i < indicator_count; ++i)
	{
		means.push_back(mean_data[i].m_data);
	}
	
	sequential_indicator_simulation_lvm(
			prop,
			grid,
			ikp,
			seed,
			&means[0],
			rep,
			use_correlograms != 0,
			simulation_mask != 0 ? simulation_mask->m_data : 0);
		
}

HPGL_API void
hpgl_simple_cokriging_mark1(
		hpgl_cont_masked_array_t * input_data,
		hpgl_cont_masked_array_t * secondary_data,
		hpgl_cokriging_m1_params_t * params,
		hpgl_cont_masked_array_t * output_data)
{
	using namespace boost;
	using namespace hpgl;
	int size = get_shape_volume(&input_data->m_shape);
	int size2 = get_shape_volume(&secondary_data->m_shape);
	int size3 = get_shape_volume(&output_data->m_shape);

	if (size != size2)
	{
		throw hpgl_exception(
				"hpgl_simple_cokriging_mark1", 
				format("Size of secondary data (%1%) is different from size of primary data (%2%)") % size2 % size);
	}

	if (size != size3)
	{
		throw hpgl_exception(
				"hpgl_simple_cokriging_mark1",
				format("Size of output data (%1%) is different from size of primary data (%2%)") % size3 % size);
	}

	cont_property_array_t primary_prop(
			input_data->m_data, input_data->m_mask, size);
	cont_property_array_t secondary_prop(
			secondary_data->m_data, secondary_data->m_mask, size2);
	cont_property_array_t output_prop(
			output_data->m_data, output_data->m_mask, size3);

	neighbourhood_param_t np;
	np.m_max_neighbours = params->m_max_neighbours;

	covariance_param_t cp;
	cp.m_nugget = params->m_nugget;
	cp.m_sill = params->m_sill;
	cp.m_covariance_type = (covariance_type_t) params->m_covariance_type;

	for (int i = 0; i < 3; ++i)
	{
		np.m_radiuses[i] = params->m_radiuses[i];
		cp.m_ranges[i] = params->m_ranges[i];
		cp.m_angles[i] = params->m_angles[i];
	}

	sugarbox_grid_t grid;
	init_grid(grid, &input_data->m_shape);

	simple_cokriging_markI(
			grid, 
			primary_prop,
			secondary_prop,
			params->m_primary_mean,
			params->m_secondary_mean,
			params->m_secondary_variance,
			params->m_correlation_coef,
			np,
			cp,
			output_prop);
						   
	
}

HPGL_API void
hpgl_simple_cokriging_mark2(
		hpgl_cont_masked_array_t * primary_data,
		hpgl_cont_masked_array_t * secondary_data,
		hpgl_cokriging_m2_params_t * params,		
		hpgl_cont_masked_array_t * output_data)
{
	using namespace boost;
	using namespace hpgl;
	int size = get_shape_volume(&primary_data->m_shape);
	int size2 = get_shape_volume(&secondary_data->m_shape);
	int size3 = get_shape_volume(&output_data->m_shape);

	if (size != size2)
	{
		throw hpgl_exception(
				"hpgl_simple_cokriging_mark1", 
				format("Size of secondary data (%1%) is different from size of primary data (%2%)") % size2 % size);
	}

	if (size != size3)
	{
		throw hpgl_exception(
				"hpgl_simple_cokriging_mark1",
				format("Size of output data (%1%) is different from size of primary data (%2%)") % size3 % size);
	}

	cont_property_array_t primary_prop(
			primary_data->m_data, primary_data->m_mask, size);
	cont_property_array_t secondary_prop(
			secondary_data->m_data, secondary_data->m_mask, size2);
	cont_property_array_t out_prop(
			output_data->m_data, output_data->m_mask, size3);

	sugarbox_grid_t grid;
	init_grid(grid, &primary_data->m_shape);

	covariance_param_t primary_cp, secondary_cp;

	init_cov_param(&primary_cp, &params->m_primary_cov_params);
	init_cov_param(&secondary_cp, &params->m_secondary_cov_params);

	neighbourhood_param_t np;
	np.m_max_neighbours = params->m_max_neighbours;
	for (int i = 0; i < 3; ++i)
	{
		np.m_radiuses[i] = params->m_radiuses[i];
	}	

	simple_cokriging_markII(
			grid, primary_prop, 
			secondary_prop,
			params->m_primary_mean,
			params->m_secondary_mean,
			params->m_correlation_coef,
			np,
			primary_cp,
			secondary_cp,
			out_prop);
}
