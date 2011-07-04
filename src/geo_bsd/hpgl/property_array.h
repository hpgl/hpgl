#ifndef __BS_PROPERTY_ARRAY_H__069FA229_B0A2_470D_84DD_385DCDBAAFC3__
#define __BS_PROPERTY_ARRAY_H__069FA229_B0A2_470D_84DD_385DCDBAAFC3__

#include <boost/smart_ptr.hpp>

#include "typedefs.h"

namespace hpgl
{
	class cont_property_array_t
	{
		float * m_data;
		unsigned char * m_mask;
		int m_size;
	public:
		cont_property_array_t(float * data, unsigned char * mask, int size)
			: m_data(data), m_mask(mask), m_size(size)
		{}

		typedef float value_type;
		typedef node_index_t size_type;
		inline size_type size()const {return m_size;}

		inline value_type operator[](size_type idx)const
		{
			return m_data[idx];
		}

		inline value_type get_at(size_type idx)const
		{
			return m_data[idx];
		}

		inline void set_at(size_type index, value_type value)
		{
			m_data[index] = value;
			m_mask[index] = 1;
		}

		bool is_informed(size_type index)const
		{
		/*	if ((index < 0) || (index >= m_size))
				return false;
			return m_mask[index] != 0; */

			return (index<0)||(index>=m_size) ? false : m_mask[index]!=0;
		}

		bool defined(node_index_t center_node, node_index_t neighbour_node)const
		{
			return is_informed(neighbour_node);
		}
	};

	class indicator_property_array_t
	{
		unsigned char  * m_data;
		unsigned char * m_mask;
		int m_size;		
	public:
		int m_indicator_count;
		indicator_property_array_t(unsigned char  * data, unsigned char * mask, int size, int indicator_count)
			: m_data(data), m_mask(mask), m_size(size), m_indicator_count(indicator_count)
		{}

		

		typedef unsigned char value_type;
		typedef node_index_t size_type;
		inline size_type size()const {return m_size;}

		inline value_type operator[](size_type idx)const
		{
			return m_data[idx];
		}

		inline value_type get_at(size_type idx)const
		{
			return m_data[idx];
		}

		inline void set_at(size_type index, value_type value)
		{
			m_data[index] = value;
			m_mask[index] = 1;
		}

		bool is_informed(size_type index)const
		{
			if ((index < 0) || (index >= m_size))
				return false;
			else
				return m_mask[index] != 0;
		}

		bool defined(node_index_t center_node, node_index_t neighbour_node)const
		{
			return is_informed(neighbour_node);
		}

		void delete_value_at(node_index_t node)
		{
			m_mask[node] = 0;
		}
	};	

	typedef boost::shared_ptr<cont_property_array_t> sp_double_property_array_t;
	typedef boost::shared_ptr<indicator_property_array_t> sp_byte_property_array_t;
	
	inline const int indicator_count(const indicator_property_array_t & prop)
	{
		return prop.m_indicator_count;
	}

	inline void set_indicator_count(indicator_property_array_t & prop, int ind_count)
	{
		prop.m_indicator_count = ind_count;
	}
	
	class indicator_array_adapter_t
	{		
		const indicator_property_array_t * m_prop;
		indicator_value_t m_value;
	public:
		typedef indicator_value_t value_type;

		indicator_array_adapter_t()
		{}

		indicator_array_adapter_t(const indicator_property_array_t * prop,	
			indicator_value_t value)
			: m_prop(prop), m_value(value)
		{}

		void init(const indicator_property_array_t * prop, indicator_value_t value)
		{
			m_prop = prop;
			m_value = value;
		}

		indicator_value_t operator[](node_index_t index)const
		{
			return m_prop->operator[](index) == m_value ? 1 : 0;			
		}		
	};
}

#endif //__BS_PROPERTY_ARRAY_H__069FA229_B0A2_470D_84DD_385DCDBAAFC3__


