/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "lvm_data.h"

namespace hpgl
{

void 
cont_lvm_data_t::assign_transfer(
		std::vector<mean_t> * data)
{
	m_data_holder.reset(data);
	m_data = m_data_holder.get();
}

indicator_lvm_data_t::indicator_lvm_data_t()
{
	
}

void
indicator_lvm_data_t::assign(const std::vector<boost::shared_ptr<std::vector<mean_t> > > &data)
{
	m_data_holder.clear();
	m_data.clear();
	for(size_t i = 0, end_i = data.size(); i < end_i; ++i)
	{
		m_data_holder.push_back(data.at(i));
		m_data.push_back(m_data_holder[i].get());
	}
}

}