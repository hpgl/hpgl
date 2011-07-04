/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "pretty_printer.h"
#include "ok_params.h"
#include "sk_params.h"
#include "ik_params.h"
#include "sgs_params.h"

namespace hpgl
{
	void print_algo_name(const std::string & name)
	{
		std::cout << "---------- Starting " << name << " ----------\n";
	}

	
	
	void print_param (const std::string & param, const std::vector<double> & value)
	{
		std::cout << param << ": [" << value [0] << ", " << value[1] << ", " << value[2] << "]\n";
	}

	void print_param (const std::string & param, const double * value)
	{
		std::cout << param << ": [" << value [0] << ", " << value[1] << ", " << value[2] << "]\n";
	}

	void print_params(const neighbourhood_param_t & p)
	{
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
	}

	void print_params(const covariance_param_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Ranges", p.m_ranges);
	}

	void print_params(const ok_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Ranges", p.m_ranges);
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
	}

	void print_params(const sk_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Ranges", p.m_ranges);
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
		if (p.m_calculate_mean)
			print_param("Mean", "automatic");
		else
			print_param("Mean", p.mean());
	}
	void print_params(const sgs_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Ranges", p.m_ranges);
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
		if (p.m_calculate_mean)
			print_param("Mean", "automatic");
		else
			print_param("Mean", p.mean());
		print_param("Seed", p.m_seed);
	}

	void add_ws(int & counter, int pos)
	{
		printf(" ");
		++counter;
		while (counter < pos)
		{
			printf(" ");
			++counter;
		}
	}

	void print_params(const ik_params_t & p)
	{
		int pos[] = {0, 2, 6, 10, 22, 34, 41, 48, 63, 71}	;
		printf("#     Cov Ranges      Angles      Sill   Nugget SearchRadiuses MaxNeib MargProb\n");
		for (indicator_index_t idx = 0; idx < p.m_category_count; ++idx)
		{
			int counter = 0;			
			counter += printf("%d", idx);
			add_ws(counter, pos[1]);
			//counter += printf("%d", p.m_values[idx]);
			add_ws(counter, pos[2]);
			counter += printf("%d", p.m_covariances[idx]);
			add_ws(counter, pos[3]);
			counter += printf("%g:%g:%g", p.m_ranges[idx][0], p.m_ranges[idx][1], p.m_ranges[idx][2]);
			add_ws(counter, pos[4]);
			counter += printf("%g:%g:%g", p.m_angles[idx][0], p.m_angles[idx][1], p.m_angles[idx][2]);
			add_ws(counter, pos[5]);
			counter += printf("%g", p.m_sills[idx]);
			add_ws(counter, pos[6]);
			counter += printf("%g", p.m_nuggets[idx]);
			add_ws(counter, pos[7]);
			counter += printf("%g:%g:%g", p.m_radiuses[idx][0], p.m_radiuses[idx][1], p.m_radiuses[idx][2]);
			add_ws(counter, pos[8]); 
			counter += printf("%d", p.m_neighbour_limits[idx]);
			add_ws(counter, pos[9]); 
			printf("%g\n", p.m_marginal_probs[idx]);				
		}				
		fflush(stdout);
	}
}