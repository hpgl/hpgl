/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <typedefs.h>

namespace hpgl
{

	boost::shared_ptr<std::vector<mean_t> > generate_complement_array(boost::shared_ptr<std::vector<mean_t> > in)
	{
		boost::shared_ptr<std::vector<mean_t> > lvm0 (new std::vector<mean_t>());
		lvm0->reserve(in->size());
		for (size_t idx = 0, end_idx = in->size(); idx < end_idx; ++idx)
		{
			lvm0->push_back(1 - in->operator[](idx));
		}
		return lvm0;
	}

}