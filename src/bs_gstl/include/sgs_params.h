/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __SGS_PARAMS_H__DD8E93BC_D7B4_4C04_A0AC_6FD6AF563A4C__
#define __SGS_PARAMS_H__DD8E93BC_D7B4_4C04_A0AC_6FD6AF563A4C__

#include <sk_params.h>



namespace hpgl
{
	enum kriging_kind_t {KRIG_ORDINARY, KRIG_SIMPLE};

	struct region_size_t
	{
		int d[3];
		int volume;
	};

	class sgs_params_t : public sk_params_t
	{
	public:
		sgs_params_t();
		kriging_kind_t m_kriging_kind;
		long int m_seed;
		mean_kind_t m_mean_kind;
		std::vector<mean_t> * m_lvm;
		bool auto_region_size;
		region_size_t reg_size;
	};
	
	inline std::ostream & operator<<(std::ostream & o, const sgs_params_t & p)
	{
		return o << (sk_params_t)p << "\tSeed: " << p.m_seed << "\n" << "Mean type: ";
		switch (p.m_mean_kind)
		{
		case e_mean_stationary_auto:
		case e_mean_stationary:
			o << "stationary";
			break;
		case e_mean_varying:
			o << "varying";
			break;
		default:
			o << "unknown";
		}
		o << "\n";
	}
}

#endif //__SGS_PARAMS_H__DD8E93BC_D7B4_4C04_A0AC_6FD6AF563A4C__

