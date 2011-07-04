/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <ik_params.h>

namespace hpgl
{
	ik_params_t::ik_params_t()
		: m_category_count(0)
	{}

	void ik_params_t::add_indicator(
			covariance_type_t covariance_type, 
			double range1, double range2, double range3, 
			double angle1, double angle2, double angle3, 
			double sill, 
			double nugget,
			double radius1, double radius2, double radius3, 
			size_t neighbour_limit, double marginal_prob)
	{
		std::vector<double> ranges;
		ranges.push_back(range1);
		ranges.push_back(range2);
		ranges.push_back(range3);

		std::vector<double> angles;
		angles.push_back(angle1);
		angles.push_back(angle2);
		angles.push_back(angle3);

		sugarbox_search_ellipsoid_t radiuses(radius1, radius2, radius3);

		m_covariances.push_back(covariance_type);
		m_ranges.push_back(ranges);
		m_angles.push_back(angles);
		m_sills.push_back(sill);
		m_nuggets.push_back(nugget);
		m_radiuses.push_back(radiuses);
		m_neighbour_limits.push_back(neighbour_limit);
		m_marginal_probs.push_back(marginal_prob);

		covariance_param_t cov_param;
		cov_param.set_angles(angle1, angle2, angle3);
		cov_param.set_ranges(range1, range2, range3);
		cov_param.m_covariance_type = covariance_type;
		cov_param.m_nugget = nugget;
		cov_param.m_sill = sill;
		m_cov_params.push_back(cov_param);

		neighbourhood_param_t nb_param;
		nb_param.set_radiuses(radius1, radius2, radius3);
		nb_param.m_max_neighbours = neighbour_limit;
		m_nb_params.push_back(nb_param);

		m_category_count++;
	}

	void ik_params_t::add_indicator(const indicator_param_t & indicator)
	{
		add_indicator(
			indicator.m_covariance_type,
			indicator.m_ranges[0],
			indicator.m_ranges[1],
			indicator.m_ranges[2],
			indicator.m_angles[0],
			indicator.m_angles[1],
			indicator.m_angles[2],
			indicator.m_sill,
			indicator.m_nugget,
			indicator.m_radiuses[0],
			indicator.m_radiuses[1],
			indicator.m_radiuses[2],
			indicator.m_max_neighbours,
			indicator.m_marginal_prob
			);
	}

	std::ostream & operator<<(std::ostream&s, const ik_params_t & p)
	{
		for (int i = 0; i < p.m_category_count; ++i)
		{
			s 
				<< "\t\tCovariance type: " << p.m_covariances[i] << "\n"
				<< "\t\tRanges:	[" << p.m_ranges[i][0] << ", " << p.m_ranges[i][1] << ", " << p.m_ranges[i][2] << "]\n"
				<< "\t\tAngles: [" << p.m_angles[i][0] << ", " << p.m_angles[i][1] << ", " << p.m_angles[i][2] << "]\n"
				<< "\t\tSill: " << p.m_sills[i] << '\n'
				<< "\t\tNugget: " << p.m_nuggets[i] << "\n"
				<< "\t\tSearch radiuses: " << p.m_radiuses[i] << "\n"
				<< "\t\tMax number of neighbours: " << p.m_neighbour_limits[i] << "\n"
				<< "\t\tMarginal probability: " << p.m_marginal_probs[i] << "\n";
		}
		return s;
	}
}
