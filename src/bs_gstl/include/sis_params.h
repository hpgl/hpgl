/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __SIS_PARAMS_H__E7505C6B_D981_4D8F_AF74_46F4B6989806__
#define __SIS_PARAMS_H__E7505C6B_D981_4D8F_AF74_46F4B6989806__
/*
#include <covariance_type.h>
#include <indicator_param.h>

namespace hpgl
{
	class BS_PLUGIN_API sis_params_t
	{
	public:
		sis_params_t();
		std::vector<covariance_type_t> m_covariances;
		std::vector<std::vector<double> > m_ranges;
		std::vector<std::vector<double> > m_angles;
		std::vector<double> m_sills;
		std::vector<double> m_nuggets;

		std::vector<std::vector<double> > m_radiuses;
		std::vector<size_t> m_neighbour_limits;

		std::vector<double> m_marginal_probs;

		std::vector<unsigned char> m_values;

		size_t m_category_count;
		long int m_seed;

		void add_indicator(
			covariance_type_t covariance_type,
			double range1, double range2, double range3,
			double angle1, double angle2, double angle3,
			double sill,
			double nugget,
			double radius1, double radius2, double radius3,
			size_t neighbour_limit,
			double marginal_prob,
			unsigned char value);

		void add_indicator(
			const indicator_param_t & indicator);
	};
}
*/
#endif //__SIS_PARAMS_H__E7505C6B_D981_4D8F_AF74_46F4B6989806__