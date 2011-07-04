/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V
#define MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V

#include "lvm_data.h"

namespace hpgl
{
	class no_mean_t
	{
	public:
		/*inline double mean(node_index_t)const
		{
			return 0;
		}*/		
		inline double operator[](node_index_t index)const
		{
			return 0;
		}
	};

	class single_mean_t
	{
		double m_mean;
	public:
		single_mean_t(double mean)
			: m_mean(mean)
		{}

		/*inline double mean(node_index_t)const
		{
			return m_mean;
		}*/
		inline double operator[](node_index_t index)const
		{
			return m_mean;
		}
	};

	void create_means(const std::vector<indicator_probability_t> & marginal_probs, std::vector<boost::shared_ptr<single_mean_t> > & means);
	
}

#endif //MEAN_PROVIDER_H_INCLUDED_T9TUHNORNUVKDSFJP3O4NP2CQ0CNPASDFJPO23RV238V
