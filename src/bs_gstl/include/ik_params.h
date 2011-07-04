/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __IK_PARAMS_H__A5485C4B_DFF9_4BA2_B8F5_B97C66A904B2__
#define __IK_PARAMS_H__A5485C4B_DFF9_4BA2_B8F5_B97C66A904B2__

#include <covariance_type.h>
#include <indicator_param.h>
#include <search_ellipsoid.h>

namespace hpgl
{
	class ik_params_t
	{
	public:
		ik_params_t();
		std::vector<covariance_type_t> m_covariances;
		std::vector<std::vector<range_t> > m_ranges;
		std::vector<std::vector<angle_t> > m_angles;
		std::vector<covariance_t> m_sills;
		std::vector<covariance_t> m_nuggets;

		std::vector<sugarbox_search_ellipsoid_t> m_radiuses;
		std::vector<size_t> m_neighbour_limits;

		std::vector<indicator_probability_t> m_marginal_probs;

		std::vector<covariance_param_t> m_cov_params;
		std::vector<neighbourhood_param_t> m_nb_params;
	
	public:
		size_t m_category_count;

		void add_indicator(
			covariance_type_t covariance_type,
			double range1, double range2, double range3,
			double angle1, double angle2, double angle3,
			double sill,
			double nugget,
			double radius1, double radius2, double radius3,
			size_t neighbour_limit,
			double marginal_prob);

		void add_indicator(
			const indicator_param_t & indicator);
	};

	std::ostream & operator<<(std::ostream&s, const ik_params_t & p);
}

#endif //__IK_PARAMS_H__A5485C4B_DFF9_4BA2_B8F5_B97C66A904B2__
