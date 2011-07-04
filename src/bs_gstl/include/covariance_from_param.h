/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __COVARIANCE_FROM_PARAM_H__8D64B282_BF1F_4BDF_83F9_5C869DE9D5E8__
#define __COVARIANCE_FROM_PARAM_H__8D64B282_BF1F_4BDF_83F9_5C869DE9D5E8__

#include <covariance_param.h>
#include <typedefs.h>
#include <covariance_model.h>

namespace hpgl
{
	class sugarbox_covariance_model_t : public covariance_model_t
	{
		sugarbox_covariance_t m_covariance;
	public:
		sugarbox_covariance_model_t(sugarbox_covariance_t cov)
			: m_covariance ( cov)
		{
		}

		virtual double operator()(coord_t & c1, coord_t & c2)
		{
			return m_covariance(
				sugarbox_location_t(c1.x(), c1.y(), c1.z()),
				sugarbox_location_t(c2.x(), c2.y(), c2.z()));
		}
	};

	void covariance_from_param(const covariance_param_t & covariance_param, sugarbox_covariance_t &);
        void real_covariance_from_param(const covariance_param_t & covariance_param, real_covariance_t &);
}

#endif //__COVARIANCE_FROM_PARAM_H__8D64B282_BF1F_4BDF_83F9_5C869DE9D5E8__
