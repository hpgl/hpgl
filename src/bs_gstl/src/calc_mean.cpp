/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <calc_mean.h>
#include <property_array.h>


	double hpgl::calc_mean(const cont_property_array_t & property)
	{	
		int count_points = 0;
		double sum = 0;

		// Mean computation procedure
		for(int idx = 0; idx < property.size(); ++idx)
		{
			if( property.is_informed(idx) )
			{
				sum += property.get_at(idx);
				count_points += 1;
			}
		}		
		return sum / count_points;
	}

namespace hpgl
{

	bool calc_mean(const cont_property_array_t * property, double * mean)
	{		
		if (!property || ! mean)
			return false;

		int count_points = 0;
		double sum = 0;
		// Mean computation procedure
		for(int idx = 0; idx < property->size(); ++idx)
		{
			if( property->is_informed(idx) )
			{
				sum += property->get_at(idx);
				count_points += 1;
			}
		}		
		*mean = sum / count_points;		
		return true;
	}
}