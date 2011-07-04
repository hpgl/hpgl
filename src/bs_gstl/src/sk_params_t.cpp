/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <sk_params.h>

namespace hpgl
{
	sk_params_t::sk_params_t()
		:m_mean(0),
		m_calculate_mean(true)
	{
	}



	std::ostream & operator <<(std::ostream & out, const sk_params_t& param)
	{
		out << (const ok_params_t&)param;

		if (param.m_calculate_mean)
			out << "\tMean: automatic\n";
		else
			out << "\tMean: " << param.mean() << "\n";

		return out;
	}
}

