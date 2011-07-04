/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <property_array.h>
#include <property_writer.h>
#include "py_ok_param.h"
#include "py_sk_params.h"
#include "py_grid.h"
#include "py_sgs_params.h"
#include <load_property_from_file.h>
#include "py_median_ik.h"
#include <hpgl_core.h>
#include "progress_reporter.h"
#include "extract_indicator_values.h"
#include "py_kriging_weights.h"

using namespace hpgl;

namespace hpgl
{
	void py_indicator_kriging(
		boost::python::tuple input_array,
		boost::python::tuple output_array,
		const py_grid_t & grid,
		PyObject * params, 
		bool use_vpc);	

	void py_sis(
		boost::python::tuple array, 
		const py_grid_t & grid,
		PyObject * params, int seed, bool use_vpc, bool use_corellogram, boost::python::object mask_data);	

	void py_sis_lvm(
			const boost::python::tuple & array,
			const py_grid_t & grid,
			PyObject * params,
			int seed,
			boost::python::object mean_data,
			bool use_corellogram,
			boost::python::object mask_data);

	namespace python
	{
		void py_read_inc_file_float(
			const std::string & filename,
			double undefined_value,
			int size,
			boost::python::object odata,
			boost::python::object omask);

		void py_read_inc_file_byte(
			const std::string & filename,
			int undefined_value,
			int size,
			boost::python::object data,
			boost::python::object mask, 
			boost::python::list ind_values);

		void py_simple_cokriging_markI(
			boost::python::tuple input_array,
			const py_grid_t & grid,
			boost::python::tuple secondary_data,
			mean_t primary_mean,
			mean_t secondary_mean,
			double secondary_variance,
			double correlation_coef,
			const boost::python::tuple & radiuses,
			int max_neighbours,
			int covariance_type,
			const boost::python::tuple & ranges,
			double sill,
			double nugget,
			const boost::python::tuple & angles,
			boost::python::tuple out_array);

		void py_simple_cokriging_markII(
			py_grid_t grid,
			boost::python::dict primary_data,
			boost::python::dict secondary_data,
			double correlation_coef,
			boost::python::tuple radiuses,
			int max_neighbours,
			boost::python::tuple out_array);		

	}
}

void write_byte_property(
		boost::python::tuple property,
		const char * filename,
		const char * name,
		double undefined_value,
		boost::python::list indicator_values)
{
	std::vector<unsigned char> remap_table;
	sp_byte_property_array_t prop = ind_prop_from_tuple(property);
	extract_indicator_values(
			indicator_values, 
			indicator_count(*prop),
			remap_table);

	hpgl::property_writer_t writer;
	writer.init(filename, name);
	writer.write_byte(prop, (unsigned char)undefined_value, remap_table);
}

void write_cont_property(
		boost::python::tuple property,
		const char * filename,
		const char * name,
		double undefined_value)
{
	sp_double_property_array_t prop = cont_prop_from_tuple(property);
	hpgl::property_writer_t writer;
	writer.init(filename, name);
	writer.write_double(prop, undefined_value);
}

// Hmm... can't we just give constructor to py_grid_t?
py_grid_t create_sugarbox_grid(int x, int y, int z)
{
	using namespace hpgl;

	py_grid_t result;

	sp_sugarbox_grid_t grid(new sugarbox_grid_t());
	grid->init(x, y, z);

	result.m_sugarbox_geometry = grid;
	return result;
}

py_ok_params_t create_ok_params()
{
	return py_ok_params_t();
}

py_sk_params_t create_sk_params()
{
	return py_sk_params_t();
}

py_sgs_params_t create_sgs_params()
{
	return py_sgs_params_t();
}

void ordinary_kriging(
		boost::python::tuple input_array, 
		boost::python::tuple output_array,
		py_grid_t grid,
		const py_ok_params_t & param, 
		bool use_new_cov)
{
	using namespace hpgl;

	sp_double_property_array_t in_prop = cont_prop_from_tuple(input_array);
	sp_double_property_array_t out_prop = cont_prop_from_tuple(output_array);
	
	hpgl::ordinary_kriging(*in_prop, *grid.m_sugarbox_geometry, param.m_ok_param, *out_prop, use_new_cov);		
}

void simple_kriging(
		boost::python::tuple input_array,
		boost::python::tuple output_array,
		const py_grid_t & grid,
		const py_sk_params_t & param)
{
	using namespace hpgl;
	sp_double_property_array_t in_prop = cont_prop_from_tuple(input_array);
	sp_double_property_array_t out_prop = cont_prop_from_tuple(output_array);
	hpgl::simple_kriging(*in_prop, *grid.m_sugarbox_geometry, param.m_sk_params, *out_prop);		
}

void lvm_kriging(
		boost::python::tuple input_array,
		boost::python::tuple output_array,
		const py_grid_t & grid,
		const py_ok_params_t & param,
		boost::python::object mean_data)
{
	using namespace hpgl;
	sp_double_property_array_t in_prop = cont_prop_from_tuple(input_array);
	sp_double_property_array_t out_prop = cont_prop_from_tuple(output_array);
	mean_t * means = get_buffer_from_ndarray<mean_t, 'f'>(mean_data, out_prop->size(), "lvm_kriging");
	hpgl::lvm_kriging(*in_prop, means, *grid.m_sugarbox_geometry, param.m_ok_param, *out_prop);	
}

void sgs_simulation(		
		boost::python::tuple output_array,
		const py_grid_t & grid,
		const py_sgs_params_t & param,		
		boost::python::object cdf_property,
		boost::python::object mask_data)
{
	using namespace boost::python;	
	sp_double_property_array_t out_prop = cont_prop_from_tuple(output_array);

	boost::python::extract<tuple> ext(cdf_property);

	unsigned char * mask =
			mask_data.ptr() == Py_None ? NULL 
			: get_buffer_from_ndarray<unsigned char,'u'>(mask_data, out_prop->size(), "py_sis");
	
	sp_double_property_array_t cdf_prop;
	if (ext.check())
	{
		cdf_prop = cont_prop_from_tuple(ext);
	}
	else
	{
		cdf_prop = out_prop;
	}

	hpgl::sequential_gaussian_simulation(*grid.m_sugarbox_geometry, param.m_sgs_params, *out_prop, *cdf_prop, mask);		
}

void sgs_lvm_simulation(
		boost::python::tuple output_array,
		const py_grid_t & grid,
		const py_sgs_params_t & param,
		boost::python::object mean_data,		
		PyObject * cdf_property,
		boost::python::object mask_data)
{
	using namespace boost::python;	
	sp_double_property_array_t out_prop = cont_prop_from_tuple(output_array);

	boost::python::extract<tuple> ext(cdf_property);
	
	unsigned char * mask =
			mask_data.ptr() == Py_None ? NULL 
			: get_buffer_from_ndarray<unsigned char,'u'>(mask_data, out_prop->size(), "sgs_lvm_simulation");

	sp_double_property_array_t cdf_prop;
	if (ext.check())
	{
		cdf_prop = cont_prop_from_tuple(ext);
	}
	else
	{
		cdf_prop = out_prop;
	}

	mean_t * lvm_data = get_buffer_from_ndarray<mean_t, 'f'>(mean_data, out_prop->size(), "sgs_lvm_simulation");
	hpgl::sequential_gaussian_simulation_lvm(*grid.m_sugarbox_geometry, param.m_sgs_params, lvm_data, *out_prop, *cdf_prop, mask);	
}

BOOST_PYTHON_MODULE(hpgl)
{
	using namespace hpgl;
	using namespace hpgl::python;
	using namespace boost::python;

	def("set_thread_num", set_thread_num);
	
	class_<py_grid_t>("grid") 
		.def(init<sugarbox_grid_size_t,
				sugarbox_grid_size_t, 
				sugarbox_grid_size_t>());

	class_<py_ok_params_t>("ok_params", init<py_ok_params_t>())
		.def("set_covariance_type", &py_ok_params_t::set_covariance_type)
		.def("set_ranges", &py_ok_params_t::set_ranges)
		.def("set_angles", &py_ok_params_t::set_angles)
		.def("set_sill", &py_ok_params_t::set_sill)
		.def("set_nugget", &py_ok_params_t::set_nugget)
		.def("set_radiuses", &py_ok_params_t::set_radiuses)
		.def("set_max_neighbours", &py_ok_params_t::set_max_neighbours);

	class_<py_sk_params_t>("sk_params", init<py_sk_params_t>())
		.def("set_covariance_type", &py_sk_params_t::set_covariance_type)
		.def("set_ranges", &py_sk_params_t::set_ranges)
		.def("set_angles", &py_sk_params_t::set_angles)
		.def("set_sill", &py_sk_params_t::set_sill)
		.def("set_nugget", &py_sk_params_t::set_nugget)
		.def("set_radiuses", &py_sk_params_t::set_radiuses)
		.def("set_max_neighbours", &py_sk_params_t::set_max_neighbours)
		.def("set_mean", &py_sk_params_t::set_mean);		

	class_<py_median_ik_params_t>("median_ik_params", init<py_median_ik_params_t>())
		.def("set_covariance_type", &py_median_ik_params_t::set_covariance_type)
		.def("set_ranges", &py_median_ik_params_t::set_ranges)
		.def("set_angles", &py_median_ik_params_t::set_angles)
		.def("set_sill", &py_median_ik_params_t::set_sill)
		.def("set_nugget", &py_median_ik_params_t::set_nugget)
		.def("set_radiuses", &py_median_ik_params_t::set_radiuses)
		.def("set_max_neighbours", &py_median_ik_params_t::set_max_neighbours)
		.def("set_marginal_probs", &py_median_ik_params_t::set_marginal_probs);

	class_<py_sgs_params_t>("sgs_params", init<py_sgs_params_t>())
		.def("set_covariance_type", &py_sgs_params_t::set_covariance_type)
		.def("set_ranges", &py_sgs_params_t::set_ranges)
		.def("set_angles", &py_sgs_params_t::set_angles)
		.def("set_sill", &py_sgs_params_t::set_sill)
		.def("set_nugget", &py_sgs_params_t::set_nugget)
		.def("set_radiuses", &py_sgs_params_t::set_radiuses)
		.def("set_max_neighbours", &py_sgs_params_t::set_max_neighbours)
		.def("set_mean", &py_sgs_params_t::set_mean)
		.def("set_kriging_kind", &py_sgs_params_t::set_kriging_kind)
		.def("set_seed", &py_sgs_params_t::set_seed)
		.def("set_mean_kind", &py_sgs_params_t::set_mean_kind);
		
	def("read_inc_file_float", py_read_inc_file_float);
	def("read_inc_file_byte", py_read_inc_file_byte);
	
	def("write_byte_property", write_byte_property);
	def("write_cont_property", write_cont_property);
	
	def("create_sugarbox_grid", create_sugarbox_grid);
	def("create_ok_params", create_ok_params);
	def("create_sk_params", create_sk_params);
	def("create_sgs_params", create_sgs_params);
	def("create_median_ik_params", py_create_median_ik_params);

    def("simple_kriging_weights", py_calculate_kriging_weight);

	def("ordinary_kriging", ::ordinary_kriging);
	def("simple_kriging", ::simple_kriging);
	def("indicator_kriging", py_indicator_kriging);
	def("sgs_simulation", sgs_simulation);
	def("sis_simulation", py_sis);
	def("sis_simulation_lvm", py_sis_lvm);
	def("lvm_kriging", ::lvm_kriging);
	def("sgs_lvm_simulation", sgs_lvm_simulation);
	def("median_ik", py_median_ik);

	def("simple_cokriging_markI", hpgl::python::py_simple_cokriging_markI);
	def("simple_cokriging_markII", hpgl::python::py_simple_cokriging_markII);
	
}
