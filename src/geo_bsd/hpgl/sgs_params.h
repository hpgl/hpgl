#ifndef __SGS_PARAMS_H__DD8E93BC_D7B4_4C04_A0AC_6FD6AF563A4C__
#define __SGS_PARAMS_H__DD8E93BC_D7B4_4C04_A0AC_6FD6AF563A4C__

#include "sk_params.h"

namespace hpgl
{
	enum kriging_kind_t {KRIG_ORDINARY=0, KRIG_SIMPLE=1};


	class sgs_params_t : public sk_params_t
	{
	public:
		sgs_params_t();
		kriging_kind_t m_kriging_kind;
		long int m_seed;
		mean_kind_t m_mean_kind;
		mean_t * m_lvm;		
		int m_min_neighbours;
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

