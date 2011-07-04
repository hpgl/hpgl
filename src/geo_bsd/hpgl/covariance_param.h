#ifndef __COVARIANCE_PARAM_H__504025C5_D9BF_48B9_A3E6_13CF529A3D6D__
#define __COVARIANCE_PARAM_H__504025C5_D9BF_48B9_A3E6_13CF529A3D6D__

#include "covariance_type.h"


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
