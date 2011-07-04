/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __LVM_DATA_H__6C233713_4E36_4BBB_8EAA_B5F2A8297011
#define __LVM_DATA_H__6C233713_4E36_4BBB_8EAA_B5F2A8297011

#include "typedefs.h"
#include "hpgl_exception.h"

namespace hpgl
{
	class cont_lvm_data_t
	{
		boost::shared_ptr<std::vector<mean_t> > m_data_holder;
		std::vector<mean_t> * m_data;
	public:
		cont_lvm_data_t();
		
		void assign_transfer(boost::shared_ptr<std::vector<mean_t> > data);
		void assign_transfer(std::vector<mean_t> * data);
		void assign_copy(const std::vector<mean_t> & data);

		inline mean_t get_at(node_index_t)const;
		inline void set_at(node_index_t, mean_t value);		
	};

	class indicator_lvm_data_t
	{
	public:
		std::vector<boost::shared_ptr<std::vector<mean_t> > > m_data_holder;
		std::vector<std::vector<mean_t>* > m_data;
	public:
		indicator_lvm_data_t();

		void assign(const std::vector<boost::shared_ptr<std::vector<mean_t> > > & data);
		void assign_transfer(const std::vector<std::vector<mean_t>*> & data);
		void assign_copy(std::vector<boost::shared_ptr<std::vector<mean_t> > > & data);
		void assign_copy(const std::vector<std::vector<mean_t>*> & data);

		inline mean_t get_at(node_index_t, indicator_index_t)const;
		inline void set_at(node_index_t, indicator_index_t, mean_t value);
		inline boost::shared_ptr<std::vector<mean_t> > for_indicator(indicator_index_t)const;

		size_t size()const
		{
			return m_data_holder.size();
		}
	};

	mean_t cont_lvm_data_t::get_at(node_index_t node)const
	{
		if (m_data)
			return (*m_data)[node];
		else
		{		
			throw hpgl_exception("cont_lvm_data::set_at", "No data.");
		}	
	}

	void cont_lvm_data_t::set_at(node_index_t node, mean_t value)
	{
		if (m_data)
			(*m_data)[node] = value;
		else
		{			
			throw hpgl_exception("cont_lvm_data::set_at", "No data.");
		}
	}

	mean_t indicator_lvm_data_t::get_at(node_index_t node, indicator_index_t indicator) const
	{
		if (m_data[indicator])
		{
			return (*(m_data[indicator]))[node];
		}
		else
		{
			throw hpgl_exception("cont_lvm_data::set_at", "No data.");
		}
	}

	void indicator_lvm_data_t::set_at(node_index_t node, indicator_index_t indicator, mean_t value)
	{
		if (m_data[indicator])
		{
			m_data[indicator]->operator [](node);
		}
		else
		{
			throw hpgl_exception("cont_lvm_data::set_at", "No data.");
		}

	}

	boost::shared_ptr<std::vector<mean_t> > indicator_lvm_data_t::for_indicator(indicator_index_t indicator) const
	{
		if (m_data.at(indicator))
			return m_data_holder[indicator];
		else
			throw hpgl_exception("cont_lvm_data::set_at", "No data.");
	}
}

#endif //__LVM_DATA_H__6C233713_4E36_4BBB_8EAA_B5F2A8297011