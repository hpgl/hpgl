/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <property_array.h>
#include <property_writer.h>
#include "py_ok_param.h"
#include "py_sk_params.h"
#include "py_property_array.h"
#include "py_grid.h"
#include "py_sgs_params.h"
#include "py_sis.h"
#include <load_property_from_file.h>
#include "py_mean_data.h"
#include "py_median_ik.h"
#include "py_lvm_data.h"
#include "py_load_property.h"
#include <hpgl_core.h>
#include "progress_reporter.h"
#include "extract_indicator_values.h"
#include "py_kriging_weights.h"

using namespace hpgl;

namespace hpgl
{
	boost::python::object 
	py_calc_vpc(py_byte_property_array_t property, py_grid_t grid, const boost::python::list & marginal_probs);

	void 
	py_save_mean_data(py_mean_data_t mean_data, const std::string & name, const std::string & property_name);

	py_byte_property_array_t py_indicator_kriging(const py_byte_property_array_t & input_array,
		const py_grid_t & grid, PyObject * params, bool use_vpc);	

	namespace python
	{
		py_cont_property_array_t py_simple_cokriging_markI(
			const py_cont_property_array_t & prop,
			const py_grid_t & grid,
			const py_cont_property_array_t & secondary_data,
			mean_t property_mean,
			mean_t secondary_mean,
			double secondary_variance,
			double correlation_coef,
			const boost::python::tuple & radiuses,
			int max_neighbours,
			int covariance_type,
			const boost::python::tuple & ranges,
			double sill,
			double nugget,
			const boost::python::tuple & angles);

		py_cont_property_array_t py_simple_cokriging_markII(
			py_grid_t grid,
			boost::python::dict primary_data,
			boost::python::dict secondary_data,
			double correlation_coef,
			boost::python::tuple radiuses,
			int max_neighbours);

		double py_calc_mean(const py_cont_property_array_t & prop);
		double py_calc_mean(const py_mean_data_t & prop);

		double (*py_calc_mean1)(const py_cont_property_array_t & prop) = &py_calc_mean;
		double (*py_calc_mean2)(const py_mean_data_t & prop) = &py_calc_mean;
	}
}

void write_byte_property(
		const py_byte_property_array_t & property,
		const char * filename,
		const char * name,
		double undefined_value,
		boost::python::list indicator_values)
{
	std::vector<unsigned char> remap_table;
	extract_indicator_values(
			indicator_values, 
			indicator_count(*property.m_byte_property_array),
			remap_table);

	hpgl::property_writer_t writer;
	writer.init(filename, name);
	writer.write_byte(property.m_byte_property_array, (unsigned char)undefined_value, remap_table);
}

void write_cont_property(
		const py_cont_property_array_t & property,
		const char * filename,
		const char * name,
		double undefined_value)
{
	hpgl::property_writer_t writer;
	writer.init(filename, name);
	writer.write_double(property.m_double_property_array, undefined_value);
}

void write_byte_gslib_property(
		const py_byte_property_array_t & property,
		const char * filename,
		const char * name,
		double undefined_value,
		int i_size, int j_size, int k_size,
		boost::python::list indicator_values)
{
	std::vector<unsigned char> remap_table;
	extract_indicator_values(
			indicator_values, 
			indicator_count(*property.m_byte_property_array),
			remap_table);

	write_gslib_byte(property.m_byte_property_array, (unsigned char)undefined_value, filename, name, i_size, j_size, k_size, remap_table);
}

void write_cont_gslib_property(
		const py_cont_property_array_t & property,
		const char * filename,
		const char * name,
		double undefined_value,
		int i_size, int j_size, int k_size)
{
	write_gslib_double(property.m_double_property_array, (unsigned char)undefined_value, filename, name, i_size, j_size, k_size);
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

py_cont_property_array_t ordinary_kriging(py_cont_property_array_t input_array, py_grid_t grid,
		const py_ok_params_t & param )
{
	using namespace hpgl;
	py_cont_property_array_t result;
	result.m_double_property_array = input_array.m_double_property_array->clone();	
	hpgl::ordinary_kriging(*input_array.m_double_property_array, *grid.m_sugarbox_geometry, param.m_ok_param, *result.m_double_property_array);	
	return result;
}

py_cont_property_array_t simple_kriging(const py_cont_property_array_t & input_array,
		const py_grid_t & grid,
		const py_sk_params_t & param)
{
	using namespace hpgl;
	py_cont_property_array_t result;
	result.m_double_property_array = input_array.m_double_property_array->clone(); 
	hpgl::simple_kriging(*input_array.m_double_property_array, *grid.m_sugarbox_geometry, param.m_sk_params, *result.m_double_property_array);	
	return result;
}

py_cont_property_array_t lvm_kriging(const py_cont_property_array_t & input_array,
		const py_grid_t & grid,
		const py_ok_params_t & param,
		const py_mean_data_t & mean_data)
{
	using namespace hpgl;
	py_cont_property_array_t result;
	result.m_double_property_array = input_array.m_double_property_array->clone();		
	hpgl::lvm_kriging(*input_array.m_double_property_array, *mean_data.m_data, *grid.m_sugarbox_geometry, param.m_ok_param, *result.m_double_property_array);	
	return result;
}

py_cont_property_array_t sgs_simulation(const py_cont_property_array_t & input_property,
		const py_grid_t & grid,
		const py_sgs_params_t & param,
		bool use_harddata,
		PyObject * cdf_property,
		boost::python::object mask_data)
{
	boost::python::extract<py_cont_property_array_t> ext(cdf_property);
	boost::python::extract<py_byte_property_array_t> mask_ext(mask_data);

	sp_double_property_array_t output_property = use_harddata ? 
		input_property.m_double_property_array->clone()
		: input_property.m_double_property_array->clone_without_data();	

	indicator_property_array_t * mask = NULL;
	if (mask_ext.check())
		mask = ((py_byte_property_array_t)mask_ext).m_byte_property_array.get();

	if(ext.check())
	{
		py_cont_property_array_t cdf_prop = ext;

		hpgl::sequential_gaussian_simulation(*grid.m_sugarbox_geometry, param.m_sgs_params, use_harddata, *output_property, *cdf_prop.m_double_property_array, mask);
	}
	else
	{
		hpgl::sequential_gaussian_simulation(*grid.m_sugarbox_geometry, param.m_sgs_params, use_harddata, *output_property, *output_property, mask);
	}

	py_cont_property_array_t result;
	result.m_double_property_array = output_property;
	return result;
}

py_cont_property_array_t sgs_lvm_simulation(const py_cont_property_array_t & input_property,
		const py_grid_t & grid,
		const py_sgs_params_t & param,
		const py_mean_data_t & mean_data,
		bool use_harddata,
		PyObject * cdf_property,
		boost::python::object mask_data)
{
	boost::python::extract<py_cont_property_array_t> ext(cdf_property);
	boost::python::extract<py_byte_property_array_t> mask_ext(mask_data);

	indicator_property_array_t * mask = mask_ext.check() 
				? ((py_byte_property_array_t) mask_ext).m_byte_property_array.get() 
				: NULL;
		
	sp_double_property_array_t output_property = use_harddata 
		?  input_property.m_double_property_array->clone()		
		: input_property.m_double_property_array->clone_without_data();

	if(ext.check())
	{
		py_cont_property_array_t cdf_prop = ext;

		hpgl::sequential_gaussian_simulation_lvm(*grid.m_sugarbox_geometry, param.m_sgs_params, use_harddata,*mean_data.m_data, *output_property,  *cdf_prop.m_double_property_array, mask);
	}
	else
	{
		hpgl::sequential_gaussian_simulation_lvm(*grid.m_sugarbox_geometry, param.m_sgs_params, use_harddata,*mean_data.m_data, *output_property,  *output_property, mask);	
	}
	py_cont_property_array_t result;
	result.m_double_property_array = output_property;
	return result;
}

BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(float)
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(float_vector_t)
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(unsigned char)
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(byte_vector_t)

BOOST_PYTHON_MODULE(hpgl)
{
	using namespace hpgl;
	using namespace boost::python;

	class_<py_byte_property_array_t>("byte_property_array", init<int, boost::python::object>())
		.def("size", &py_byte_property_array_t::size)
		.def("get_at", &py_byte_property_array_t::get_at)
		.def("set_at", &py_byte_property_array_t::set_at)
		.def("is_informed", &py_byte_property_array_t::is_informed)
		.def("clone", &py_byte_property_array_t::clone)
		.def("get_raw_byte_pointer", &py_byte_property_array_t::get_raw_byte_pointer, return_value_policy<return_opaque_pointer>())
		.def("get_raw_byte_vector_pointer", &py_byte_property_array_t::get_raw_byte_vector_pointer, return_value_policy<return_opaque_pointer>());

	class_<py_cont_property_array_t>("cont_property_array", init<int>())
		.def("size", &py_cont_property_array_t::size)
		.def("get_at", &py_cont_property_array_t::get_at)
		.def("set_at", &py_cont_property_array_t::set_at)
		.def("is_informed", &py_cont_property_array_t::is_informed)
		.def("clone", &py_cont_property_array_t::clone)
		.def("get_raw_float_pointer", &py_cont_property_array_t::get_raw_float_pointer, return_value_policy<return_opaque_pointer>())
		.def("get_raw_float_vector_pointer", &py_cont_property_array_t::get_raw_float_vector_pointer, return_value_policy<return_opaque_pointer>());

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
		.def("set_mean_kind", &py_sgs_params_t::set_mean_kind)
		.def("set_auto_reg", &py_sgs_params_t::set_auto_reg)
		.def("set_region_size", &py_sgs_params_t::set_region_size);

	class_<py_mean_data_t>("MeanData", init<py_mean_data_t>());
	class_<py_indicator_lvm_data_t>("IndicatorLvmData", init<py_indicator_lvm_data_t>());
				
	def("load_cont_property", load_cont_property);
	def("load_ind_property", load_ind_property);
	def("load_gslib_cont_property", load_gslib_cont_property);
	def("load_gslib_ind_property", load_gslib_ind_property);

	def("load_mean_data", py_load_mean_data);
	def("load_indicator_mean_data", py_load_indicator_mean_data);

	def("write_byte_property", write_byte_property);
	def("write_cont_property", write_cont_property);
	def("write_byte_gslib_property", write_byte_gslib_property);
	def("write_cont_gslib_property", write_cont_gslib_property);
	def("write_mean_data", py_save_mean_data);

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

	def("calc_vpc", py_calc_vpc);
	def("calc_mean", hpgl::python::py_calc_mean1);
	def("calc_mean", hpgl::python::py_calc_mean2);
}
