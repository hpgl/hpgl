/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"
#include "py_property_array.h"

namespace hpgl
{
	// .size()
	
	int py_cont_property_array_t::size()
	{
		return m_double_property_array->size();
	}
	
	int py_byte_property_array_t::size()const
	{
		return m_byte_property_array->size();
	}

	// .is_informed()

	bool py_cont_property_array_t::is_informed(int index)const
	{
		return m_double_property_array->is_informed(index);
	}
	
	bool py_byte_property_array_t::is_informed(int index)const
	{
		return m_byte_property_array->is_informed(index);
	}

	// .get_at()

	indicator_value_t py_byte_property_array_t::get_at(int index)const
	{
		return m_byte_property_array->get_at(index);
	}

	cont_value_t py_cont_property_array_t::get_at(int index)const
	{
		return m_double_property_array->get_at(index);
	}

	// .set_at()

	void py_cont_property_array_t::set_at(int index, double value)
	{
		return m_double_property_array->set_at(index,value);
	}

	void py_byte_property_array_t::set_at(int index, int value)
	{
		return m_byte_property_array->set_at(index,value);
	}
}




