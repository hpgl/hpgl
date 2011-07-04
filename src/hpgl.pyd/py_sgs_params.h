/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__
#define __PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__

#include <sgs_params.h>

namespace hpgl
{
	class py_sgs_params_t
	{
	public:
		sgs_params_t m_sgs_params;
		void set_covariance_type(int type);
		void set_ranges(double r1, double r2, double r3);
		void set_angles(double a1, double a2, double a3);
		void set_sill(double sill);
		void set_nugget(double nugget);
		void set_radiuses(size_t , size_t , size_t );	
		void set_max_neighbours(size_t mn);
		void set_mean(double mean);
		void set_kriging_kind(int kind);
		void set_seed(long int seed);
		void set_mean_kind(const std::string & mean_kind);
		void set_auto_reg(bool auto_reg);
		void set_region_size(size_t, size_t, size_t);
	};
}

#endif //__PY_SGS_PARAMS_H__A82C5906_7AE1_4CBE_804A_4F004CCACD5F__
