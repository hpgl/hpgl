#include "stdafx.h"
#include "covariance_param.h"


namespace hpgl
{
	covariance_param_t::covariance_param_t()
	{
		m_sill = 0;
		m_nugget = 0;
		m_covariance_type = covariance_type_t::COV_SPHERICAL;
		set_ranges(0, 0, 0);
		set_angles(0, 0, 0);
	}
	void covariance_param_t::set_ranges(double range1, double range2, double range3)
	{
		m_ranges[0] = range1;
		m_ranges[1] = range2;
		m_ranges[2] = range3;
	}

	void covariance_param_t::set_angles(double angle1, double angle2, double angle3)
	{
		m_angles[0] = angle1;
		m_angles[1] = angle2;
		m_angles[2] = angle3;
	}
}