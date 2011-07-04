/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__
#define __PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__

#include <ok_params.h>

namespace hpgl
{
	class py_ok_params_t
	{
	public:
		ok_params_t m_ok_param;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
	};
	
}

#endif //__PY_OK_PARAM_H__6B03BE7E_96D2_463A_BCD7_BAE4C656C01F__
