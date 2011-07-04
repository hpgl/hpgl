#ifndef __SK_PARAMS_H__1231DE42_FECC_4177_9846_3244169F5E4C__
#define __SK_PARAMS_H__1231DE42_FECC_4177_9846_3244169F5E4C__

#include "ok_params.h"

namespace hpgl
{
	
	class sk_params_t : public ok_params_t
	{
	public:
		sk_params_t();
	private:
		double m_mean;
	public:
		double mean()const {return m_mean;}
		void set_mean(double mean)
		{
			m_mean = mean;
			m_calculate_mean = false;
		}

		bool m_calculate_mean;
	};

	std::ostream & operator <<(std::ostream &, const sk_params_t&);
}

#endif //__SK_PARAMS_H__1231DE42_FECC_4177_9846_3244169F5E4C__
