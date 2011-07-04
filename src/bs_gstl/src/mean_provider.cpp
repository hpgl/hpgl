/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "mean_provider.h"
#include "hpgl_exception.h"

namespace hpgl
{
	void create_means(
			const std::vector<indicator_probability_t> & marginal_probs, 
			std::vector<boost::shared_ptr<single_mean_t> > & means)
	{
		for (indicator_index_t i = 0; i < marginal_probs.size(); ++i)
			means.push_back(
				boost::shared_ptr<single_mean_t>(
					new single_mean_t(marginal_probs[i])));
	}
		
}
