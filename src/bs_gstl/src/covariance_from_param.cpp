/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <hpgl_exception.h>

#include <covariance_from_param.h>


namespace hpgl {
	
 
void covariance_from_param(const covariance_param_t & param, sugarbox_covariance_t & result)
{	
	if (result.structures_count() != 0)
		throw hpgl_exception("covariance_from_param", "Covariance already contains structures. Clean one required.");


	result.nugget(param.m_nugget);

	switch( param.m_covariance_type )
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
		assert(false);
	}

	result.set_ranges(
		0,
		param.m_ranges[0],
		param.m_ranges[1],
		param.m_ranges[2]);

	result.set_angles(
		0,
		param.m_angles[0],
		param.m_angles[1],
		param.m_angles[2]);

	result.sill(0, param.m_sill);	
}
void real_covariance_from_param(const covariance_param_t & param, real_covariance_t & result)
{
	if (result.structures_count() != 0)
		throw hpgl_exception("covariance_from_param", "Covariance already contains structures. Clean one required.");


	result.nugget(param.m_nugget);

	switch( param.m_covariance_type )
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
		assert(false);
	}

	result.set_ranges(
		0,
		param.m_ranges[0],
		param.m_ranges[1],
		param.m_ranges[2]);

	result.set_angles(
		0,
		param.m_angles[0],
		param.m_angles[1],
		param.m_angles[2]);

	result.sill(0, param.m_sill);
}

}
