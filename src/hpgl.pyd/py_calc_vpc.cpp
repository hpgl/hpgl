/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "typedefs.h"
#include "py_property_array.h"
#include "property_array.h"
#include "py_grid.h"
#include "py_mean_data.h"
#include "hpgl_exception.h"

namespace hpgl
{	
/*	Base for future implementation of vpc for continuous property
void calc_cont_vpc(const indicator_property_array_t & prop, sugarbox_grid_t & grid, std::vector<double> & result)
	{
	}

	void calc_ind_vpc(const cont_property_array_t & prop, sugarbox_grid_t & grid, const std::vector<double> & marginal_probs, std::vector<double> & result)
	{
		std::cout << "Calculating VPC...\n";
						
		sugarbox_grid_size_t x, y, z;
		grid.get_dimensions(x, y, z);
		
		const std::vector<indicator_value_t> & values = indicator_values(prop);
		size_t indicator_count = values.size();

		int marginal_prob_count = marginal_probs.size();
		std::vector<double> probs = marginal_probs;

		if (indicator_count == 2 && marginal_prob_count == 1)
		{			
			probs.push_back(1 - probs[0]);
		}
		else if (indicator_count != marginal_prob_count)
		{
			throw hpgl_exception("hpgl::py_calc_vpc", boost::format("Property has %s indicators, but %s probabilites is given.") % indicator_count % marginal_prob_count);
		}
		
		std::vector<boost::shared_ptr<std::vector<mean_t> > > lvms;
		for (indicator_index_t idx = 0; idx < indicator_count; ++idx)
		{
			lvms.push_back(boost::shared_ptr<std::vector<mean_t> >(new std::vector<mean_t>(prop.size())));
		}
		
		int layers_with_data = 0;
		int layers_without_data = 0;
		for (sugarbox_grid_size_t k = 0; k < z; ++k)
		{
			std::vector<int> counters(indicator_count, 0);
			int informed_nodes = 0;
			for (sugarbox_grid_size_t j = 0; j < y; ++j)
			{
				for (sugarbox_grid_size_t i = 0; i < x; ++i)
				{
					node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
					if (prop.is_informed(node))
					{
						indicator_value_t value = prop.get_at(node);
						informed_nodes += 1;
						for (indicator_index_t idx = 0; idx < indicator_count; ++idx)
						{
							if (value == values[idx])
							{
								counters[idx] += 1; 
							}
						}
					}
				}
			}
					
			if (informed_nodes > 0)
			{
				layers_with_data++;
				for (sugarbox_grid_size_t j = 0; j < y; ++j)
					for (sugarbox_grid_size_t i = 0; i < x; ++i)
					{
						node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
						for (indicator_index_t idx = 0; idx < indicator_count; ++idx)
							lvms[idx]->operator[](node) = ((double)counters[idx]) / ((double) informed_nodes);							
					}
			}
			else
			{
				layers_without_data++;
				for (sugarbox_grid_size_t j = 0; j < y; ++j)
					for (sugarbox_grid_size_t i = 0; i < x; ++i)
					{
						node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
						for (indicator_index_t idx = 0; idx < indicator_count; ++idx)
							lvms[idx]->operator[](node) = probs[idx];
					}
			}
		}

		for (indicator_index_t idx = 0; idx < indicator_count; ++idx)
		{
			py_mean_data_t md;
			md.m_data = lvms[idx];
			result.append(md);
		}		

		std::cout << boost::format("VPC has been calculated.\nGrid size: %s x %s x %s\nIndicator count: %s\nLayers with data: %s\nLayers without data: %s\n") % x % y % z % indicator_count % layers_with_data % layers_without_data;
		
		return result;
	} */

	boost::python::object py_calc_vpc(py_byte_property_array_t property, py_grid_t grid, const boost::python::list & marginal_probs)
	{
		boost::python::list result;
		indicator_property_array_t & prop = *property.m_byte_property_array;
		
		sugarbox_grid_size_t x, y, z;
		grid.m_sugarbox_geometry->get_dimensions(x, y, z);
		
		//const std::vector<indicator_value_t> & values = indicator_values(prop);
		size_t ind_count = indicator_count(prop);

		int marginal_prob_count = boost::python::len(marginal_probs);
		std::vector<double> probs;

		if (ind_count == 2 && marginal_prob_count == 1)
		{
			double prob1 = boost::python::extract<double>(marginal_probs[0]);
			double prob2 = 1 - prob1;
			probs.push_back(prob1);
			probs.push_back(prob2);
		}
		else if (ind_count == marginal_prob_count)
		{
			for (size_t idx = 0; idx < ind_count; ++idx)
			{
				probs.push_back(boost::python::extract<double>(marginal_probs[idx]));
			}
		} 
		else
		{
			throw hpgl_exception("hpgl::py_calc_vpc", boost::format("Property has %s indicators, but %s probabilites is given.") % ind_count % marginal_prob_count);
		}
		
		std::vector<boost::shared_ptr<std::vector<mean_t> > > lvms;
		for (indicator_index_t idx = 0; idx < ind_count; ++idx)
		{
			lvms.push_back(boost::shared_ptr<std::vector<mean_t> >(new std::vector<mean_t>(prop.size())));
		}
		
		int layers_with_data = 0;
		int layers_without_data = 0;
		for (sugarbox_grid_size_t k = 0; k < z; ++k)
		{
			std::vector<int> counters(ind_count, 0);
			int informed_nodes = 0;
			for (sugarbox_grid_size_t j = 0; j < y; ++j)
			{
				for (sugarbox_grid_size_t i = 0; i < x; ++i)
				{
					node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
					if (prop.is_informed(node))
					{
						indicator_value_t value = prop.get_at(node);
						informed_nodes += 1;
						for (indicator_index_t idx = 0; idx < ind_count; ++idx)
						{
							if (value == idx)
							{
								counters[idx] += 1; 
							}
						}
					}
				}
			}
					
			if (informed_nodes > 0)
			{
				layers_with_data++;
				for (sugarbox_grid_size_t j = 0; j < y; ++j)
					for (sugarbox_grid_size_t i = 0; i < x; ++i)
					{
						node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
						for (indicator_index_t idx = 0; idx < ind_count; ++idx)
							lvms[idx]->operator[](node) = ((double)counters[idx]) / ((double) informed_nodes);							
					}
			}
			else
			{
				layers_without_data++;
				for (sugarbox_grid_size_t j = 0; j < y; ++j)
					for (sugarbox_grid_size_t i = 0; i < x; ++i)
					{
						node_index_t node = grid.m_sugarbox_geometry->get_index(sugarbox_location_t(i,j,k));
						for (indicator_index_t idx = 0; idx < ind_count; ++idx)
							lvms[idx]->operator[](node) = probs[idx];
					}
			}
		}

		for (indicator_index_t idx = 0; idx < ind_count; ++idx)
		{
			py_mean_data_t md;
			md.m_data = lvms[idx];
			result.append(md);
		}		

		std::cout << boost::format("VPC has been calculated.\nGrid size: %s x %s x %s\nIndicator count: %s\nLayers with data: %s\nLayers without data: %s\n") % x % y % z % ind_count % layers_with_data % layers_without_data;
		
		return result;
	}
}