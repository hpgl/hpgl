/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __SK_PARAMS_H__1231DE42_FECC_4177_9846_3244169F5E4C__
#define __SK_PARAMS_H__1231DE42_FECC_4177_9846_3244169F5E4C__

#include <ok_params.h>

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
