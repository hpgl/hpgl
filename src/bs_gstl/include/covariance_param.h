/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __COVARIANCE_PARAM_H__504025C5_D9BF_48B9_A3E6_13CF529A3D6D__
#define __COVARIANCE_PARAM_H__504025C5_D9BF_48B9_A3E6_13CF529A3D6D__

#include <covariance_type.h>


namespace hpgl
{
	class covariance_param_t
	{
	public:
		covariance_param_t();
		covariance_type_t m_covariance_type;
		double m_ranges[3];
		double m_angles[3];
		double m_sill;
		double m_nugget;

		void set_ranges(double range1,
			double range2, double range3);

		void set_angles(double angle1,
			double angle2, double angle3);
	};


}

#endif //__COVARIANCE_PARAM_H__504025C5_D9BF_48B9_A3E6_13CF529A3D6D__
