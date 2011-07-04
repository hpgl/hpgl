/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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




