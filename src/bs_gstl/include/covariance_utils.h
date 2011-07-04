/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef COVARIANCE_UTILS_H_INCLUDED_ASDJLSDAJF87923749823NFAJDFH8723423FKDHASFHK273478923FHKDSJHFIWAE423749WEKJHKSDFSKFHKSD
#define COVARIANCE_UTILS_H_INCLUDED_ASDJLSDAJF87923749823NFAJDFH8723423FKDHASFHK273478923FHKDSJHFIWAE423749WEKJHKSDFSKFHKSD

#include <typedefs.h>
#include <covariance_type.h>
#include <hpgl_exception.h>
#include "covariance_param.h"

namespace hpgl
{
	template<typename T>
	void create_covariance(
			covariance_type_t type,
			const std::vector<range_t> ranges,
			const std::vector<angle_t> angles,
			double sill,
			double nugget,
			Covariance<T> & result)
	{
		result.nugget(nugget);

		switch(type)
		{

		case COV_SPHERICAL :
			result.add_structure("Spherical");
			break;
		case COV_EXPONENTIAL :
			result.add_structure("Exponential");
			break;
		case COV_GAUSSIAN:
			result.add_structure("Gaussian");
			break;
		default:
			throw hpgl_exception("create_covariance", "Invalid covariance type.");
		}

		result.set_ranges(0,
			ranges[0],
			ranges[1],
			ranges[2]);

		result.set_angles(0,
			angles[0],
			angles[1],
			angles[2]);

		result.sill(0, sill);
	}

	template<typename T>
	void create_covariance(const covariance_param_t & param, Covariance<T> & result)
	{
		result.nugget(param.m_nugget);

		switch(param.m_covariance_type)
		{

		case COV_SPHERICAL :
			result.add_structure("Spherical");
			break;
		case COV_EXPONENTIAL :
			result.add_structure("Exponential");
			break;
		case COV_GAUSSIAN:
			result.add_structure("Gaussian");
			break;
		default:
			throw hpgl_exception("create_covariance", "Invalid covariance type.");
		}

		result.set_ranges(0,
			param.m_ranges[0],
			param.m_ranges[1],
			param.m_ranges[2]);

		result.set_angles(0,
			param.m_angles[0],
			param.m_angles[1],
			param.m_angles[2]);

		result.sill(0, param.m_sill);
	}
}

#endif // COVARIANCE_UTILS_H_INCLUDED_ASDJLSDAJF87923749823NFAJDFH8723423FKDHASFHK273478923FHKDSJHFIWAE423749WEKJHKSDFSKFHKSD
