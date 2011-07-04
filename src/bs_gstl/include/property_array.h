/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __BS_PROPERTY_ARRAY_H__069FA229_B0A2_470D_84DD_385DCDBAAFC3__
#define __BS_PROPERTY_ARRAY_H__069FA229_B0A2_470D_84DD_385DCDBAAFC3__

#include <boost/smart_ptr.hpp>

#include <typedefs.h>

namespace hpgl
{
	template<typename T>
	class property_array_const_iterator_t;

	template<typename T> struct property_custom_data_t
	{	};

	template<>
	struct property_custom_data_t<indicator_value_t>
	{
		int m_indicator_count;
	};	

	template<typename T>
	class property_array_t
	{		
		friend const int indicator_count(const property_array_t<indicator_value_t> & prop);
		friend void set_indicator_count(property_array_t<indicator_value_t> & prop, int ind_count);
		enum flags_t
		{
			PRFL_INFORMED = 1,
			PRFL_HARDDATA = 2
		};
		std::vector<T> data_;
		std::vector<unsigned char> m_flags;
		std::string m_name;
		size_t m_size;
		property_custom_data_t<T> m_property_data;
		property_array_t();
	public:
		typedef T value_type;
		typedef node_index_t size_type;
		property_array_t(size_type size);	
		property_array_t(T value, size_type size);	
		property_array_t(const property_array_t<T> & array);
		~property_array_t();

		boost::shared_ptr<property_array_t<T> > clone()const;
		boost::shared_ptr<property_array_t<T> > clone_without_data()const;

		void assign(const std::vector<T> & data, T undefined_value);

		void copyto(property_array_t<T> & dest);

		void turn_informed_to_harddata();
		size_type size()const;

		const std::string & name();
		void set_name(const std::string & name);
		
		const std::vector<T> & data()const
			{return data_;}

		inline T operator[](size_type index)const
		{
			return get_at(index);
		}

		T get_at(size_type index)const;
		void set_at(size_type index, T value);
		inline bool is_informed(size_type index)const;	
		inline bool defined(node_index_t center_node, node_index_t neighbour_node)const
		{
			return is_informed(neighbour_node);
		}			
	};

	typedef property_array_t<cont_value_t> cont_property_array_t;
	typedef property_array_t<indicator_value_t> indicator_property_array_t;
	typedef boost::shared_ptr<cont_property_array_t> sp_double_property_array_t;
	typedef boost::shared_ptr<indicator_property_array_t> sp_byte_property_array_t;

	template<typename T>
	bool property_array_t<T>::is_informed(
		typename property_array_t<T>::size_type index) const
	{
		if ((unsigned) index < (unsigned) m_size)
			return (m_flags[index] & PRFL_INFORMED) != 0;
		else
			return false;
	}
	
	inline const int indicator_count(const indicator_property_array_t & prop)
	{
		return prop.m_property_data.m_indicator_count;
	}

	inline void set_indicator_count(property_array_t<indicator_value_t> & prop, int ind_count)
	{
		prop.m_property_data.m_indicator_count = ind_count;
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


