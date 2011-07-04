/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


// bs_property_array.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <property_array.h>

using namespace hpgl;

template<typename T>
property_array_t<T>::property_array_t()
: m_size(-50)
{	
}

template<typename T>
property_array_t<T>::property_array_t(size_type size)
: m_size(size)
{
	data_.resize(size);
	m_flags.resize(size, 0);
}

template<typename T>
property_array_t<T>::property_array_t(T value, size_type size)
: m_size(size)
{
	data_.resize(size, value);
	m_flags.resize(size, PRFL_INFORMED);
}

template<typename T>
property_array_t<T>::property_array_t(
		const property_array_t<T> & array)
: m_size(array.m_size)
{
	data_.assign(
			array.data_.begin(),
			array.data_.end());
	m_flags.assign(array.m_flags.begin(),
		array.m_flags.end());
	m_property_data = array.m_property_data;
}

template<typename T>
boost::shared_ptr<property_array_t<T> > property_array_t<T>::clone()const
{
	boost::shared_ptr<property_array_t<T> > result(new property_array_t<T>());
	result->m_size = m_size;
	result->data_ = data_;
	result->m_flags = m_flags;
	result->m_property_data = m_property_data;
	return result;
}

template<typename T>
boost::shared_ptr<property_array_t<T> > property_array_t<T>::clone_without_data()const
{
	boost::shared_ptr<property_array_t<T> > result(new property_array_t<T>());
	result->m_size = m_size;
	result->data_.resize(m_size);
	result->m_flags.resize(m_size, 0);
	result->m_property_data = m_property_data;
	return result;
}

template<typename T>
property_array_t<T>::~property_array_t()
{
}

template<typename T>
void 
property_array_t<T>::assign(
		const std::vector<T> & data, 
		T undefined_value)
{
	m_size = data.size();
	data_.assign(data.begin(), data.end());
	m_flags.resize(data.size());
	for (size_t idx = 0, end_idx = data_.size();
			idx < end_idx;
			++idx)
	{
		if (data_[idx] == undefined_value)
			m_flags[idx] = 0;
		else
			m_flags[idx] = PRFL_INFORMED | PRFL_HARDDATA;	
	}
}

template<typename T>
void
property_array_t<T>::copyto(property_array_t<T> & dest)
{
	dest.data_.assign(data_.begin(), data_.end());
	dest.m_flags.assign(m_flags.begin(), m_flags.end());
	dest.m_size = m_size;
}

template<typename T>
void
property_array_t<T>::turn_informed_to_harddata()
{
	for (size_t idx = 0, end_idx = m_flags.size();
			idx < end_idx; ++idx)
	{
		if (m_flags[idx] & PRFL_INFORMED)
			m_flags[idx] |= PRFL_HARDDATA;
	}
}

template<typename T>
bs_node_index_t
property_array_t<T>::size()const
{
	assert(m_size == data_.size());
	return m_size;
}

template<typename T>
T property_array_t<T>::get_at(size_type index)const
{
	//return data_.at(index);
	return data_[index];
}

template<typename T>
void property_array_t<T>::set_at(
	typename property_array_t<T>::size_type index, 
	T value)
{
	data_.at(index) = value;
	m_flags.at(index) |= PRFL_INFORMED;
}

template class property_array_t<indicator_value_t>;
template class property_array_t<cont_value_t>;


