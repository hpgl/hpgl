/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include "py_grid.h"
#include "neighbourhood_param.h"
#include "covariance_param.h"
#include <hpgl_core.h>

#include "numpy_utils.h"


namespace hpgl
{
	namespace python
	{
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
			boost::python::tuple out_array)
		{
			if (boost::python::len(radiuses) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(radiuses) = %s. Should be 3") % boost::python::len(radiuses));
			if (boost::python::len(ranges) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(ranges) = %s. Should be 3") % boost::python::len(ranges));
			if (boost::python::len(angles) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(angles) = %s. Should be 3") % boost::python::len(angles));
			
			neighbourhood_param_t np; 
			np.m_max_neighbours = max_neighbours;

			covariance_param_t pcp;  
			pcp.m_nugget = nugget;
			pcp.m_sill = sill;
			pcp.m_covariance_type = (covariance_type_t) covariance_type;
			
			for (int i = 0; i < 3; ++i)
			{
				np.m_radiuses[i] = boost::python::extract<int>(radiuses[i]);
			       	pcp.m_ranges[i] = boost::python::extract<double>(ranges[i]);
		       		pcp.m_angles[i] = boost::python::extract<double>(angles[i]);
			}

			sp_double_property_array_t primary_prop = cont_prop_from_tuple(input_array);
			sp_double_property_array_t secondary_prop = cont_prop_from_tuple(secondary_data);
			sp_double_property_array_t out_prop = cont_prop_from_tuple(out_array);


			simple_cokriging_markI(*grid.m_sugarbox_geometry, *primary_prop, 
				*secondary_prop, primary_mean, secondary_mean, 
				secondary_variance, correlation_coef, np, pcp, *out_prop);			
		}

		void py_simple_cokriging_markII(
			py_grid_t grid,
			boost::python::dict primary_data,
			boost::python::dict secondary_data,
			double correlation_coef,
			boost::python::tuple radiuses,
			int max_neighbours,
			boost::python::tuple out_array)
		{
			using namespace boost::python;
			sp_double_property_array_t input_prop = 
				cont_prop_from_tuple(extract<tuple>(primary_data["data"]));
			sp_double_property_array_t secondary_prop =
				cont_prop_from_tuple(extract<tuple>(secondary_data["data"]));
			sp_double_property_array_t out_prop =
				cont_prop_from_tuple(out_array);
						
			mean_t primary_mean = extract<mean_t>(primary_data["mean"]);
			mean_t secondary_mean = extract<mean_t>(secondary_data["mean"]);
			
			object primary_cov_model = primary_data["cov_model"];
			object secondary_cov_model = secondary_data["cov_model"];
			
			boost::python::object primary_ranges = primary_cov_model.attr("ranges");
			boost::python::object secondary_ranges = secondary_cov_model.attr("ranges");

			boost::python::object primary_angles = primary_cov_model.attr("angles");
			boost::python::object secondary_angles = secondary_cov_model.attr("angles");

			double primary_sill = boost::python::extract<double>(primary_cov_model.attr("sill"));
			double secondary_sill = boost::python::extract<double>(secondary_cov_model.attr("sill"));

			double primary_nugget = boost::python::extract<double>(primary_cov_model.attr("nugget"));
			double secondary_nugget = boost::python::extract<double>(secondary_cov_model.attr("nugget"));

			covariance_type_t primary_cov_type =  (covariance_type_t)((int)boost::python::extract<int>(primary_cov_model.attr("type")));
			covariance_type_t secondary_cov_type =   (covariance_type_t)((int)boost::python::extract<int>(secondary_cov_model.attr("type")));

			covariance_param_t primary_cov_params;
			covariance_param_t secondary_cov_params;
			neighbourhood_param_t np;
			np.m_max_neighbours = max_neighbours;

			for (int i = 0; i < 3; ++i)
			{
				np.m_radiuses[i] = boost::python::extract<int>(radiuses[i]);
			    primary_cov_params.m_ranges[i] = boost::python::extract<double>(primary_ranges[i]);
		       	primary_cov_params.m_angles[i] = boost::python::extract<double>(primary_angles[i]);
				secondary_cov_params.m_ranges[i] = boost::python::extract<double>(secondary_ranges[i]);
		       	secondary_cov_params.m_angles[i] = boost::python::extract<double>(secondary_angles[i]);
			}
			primary_cov_params.m_sill = primary_sill;
			primary_cov_params.m_nugget = primary_nugget;
			primary_cov_params.m_covariance_type = primary_cov_type;
			
			secondary_cov_params.m_sill = secondary_sill;
			secondary_cov_params.m_nugget = secondary_nugget;
			secondary_cov_params.m_covariance_type = secondary_cov_type;
			
			simple_cokriging_markII(
				*grid.m_sugarbox_geometry, 
				*input_prop, 
				*secondary_prop, 
				primary_mean, secondary_mean, correlation_coef, np, primary_cov_params, secondary_cov_params, *out_prop);			
		}

	}
}
