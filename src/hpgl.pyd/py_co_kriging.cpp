/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include "py_property_array.h"
#include "py_grid.h"
#include "py_mean_data.h"
#include "neighbourhood_param.h"
#include "covariance_param.h"
#include <hpgl_core.h>


namespace hpgl
{
	namespace python
	{
		py_cont_property_array_t py_simple_cokriging_markI(
			const py_cont_property_array_t & prop,
			const py_grid_t & grid,
			const py_cont_property_array_t & secondary_data,
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
			const boost::python::tuple & angles)
		{
			if (boost::python::len(radiuses) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(radiuses) = %s. Should be 3") % boost::python::len(radiuses));
			if (boost::python::len(ranges) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(ranges) = %s. Should be 3") % boost::python::len(ranges));
			if (boost::python::len(angles) != 3)
				throw hpgl_exception("py_simple_cokriging_markI", boost::format("len(angles) = %s. Should be 3") % boost::python::len(angles));

			py_cont_property_array_t result;
			result.m_double_property_array = prop.m_double_property_array->clone();

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

			

			simple_cokriging_markI(*grid.m_sugarbox_geometry, *prop.m_double_property_array, *secondary_data.m_double_property_array,
					primary_mean, secondary_mean, secondary_variance, correlation_coef, np, pcp, *result.m_double_property_array);
			return result;
		}

		py_cont_property_array_t py_simple_cokriging_markII(
			py_grid_t grid,
			boost::python::dict primary_data,
			boost::python::dict secondary_data,
			double correlation_coef,
			boost::python::tuple radiuses,
			int max_neighbours)
		{
			py_cont_property_array_t input_prop = boost::python::extract<py_cont_property_array_t>(primary_data["data"]);
			py_cont_property_array_t secondary_prop = boost::python::extract<py_cont_property_array_t>(secondary_data["data"]);
			
			mean_t primary_mean = boost::python::extract<mean_t>(primary_data["mean"]);
			mean_t secondary_mean = boost::python::extract<mean_t>(secondary_data["mean"]);

			boost::python::object primary_ranges = primary_data["ranges"];
			boost::python::object secondary_ranges = secondary_data["ranges"];

			boost::python::object primary_angles = primary_data["angles"];
			boost::python::object secondary_angles = secondary_data["angles"];

			double primary_sill = boost::python::extract<double>(primary_data["sill"]);
			double secondary_sill = boost::python::extract<double>(secondary_data["sill"]);

			double primary_nugget = boost::python::extract<double>(primary_data["nugget"]);
			double secondary_nugget = boost::python::extract<double>(secondary_data["nugget"]);

			covariance_type_t primary_cov_type =  (covariance_type_t)((int)boost::python::extract<int>(primary_data["cov_type"]));
			covariance_type_t secondary_cov_type =   (covariance_type_t)((int)boost::python::extract<int>(secondary_data["cov_type"]));

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

			py_cont_property_array_t result;
			result.m_double_property_array = input_prop.m_double_property_array->clone();

			simple_cokriging_markII(
				*grid.m_sugarbox_geometry, 
				*input_prop.m_double_property_array, 
				*secondary_prop.m_double_property_array, 
				primary_mean, secondary_mean, correlation_coef, np, primary_cov_params, secondary_cov_params, *result.m_double_property_array);

			return result;
		}

	}
}
