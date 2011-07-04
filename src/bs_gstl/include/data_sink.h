/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef DATA_SINK_H_INCLUDED_ERTGHJKFGHUJIE4R5U689CVFGYH3456U3ERDFGFDFGHJJMHGRTYUISDFGPOIUYSAGHK765RTDF7TYRT9KLVB
#define DATA_SINK_H_INCLUDED_ERTGHJKFGHUJIE4R5U689CVFGYH3456U3ERDFGFDFGHJJMHGRTYUISDFGPOIUYSAGHK765RTDF7TYRT9KLVB

#include <property_array.h>

namespace hpgl
{
	class data_sink_t
	{
	public:
		virtual void add_value(double) = 0;
		virtual void flush() = 0;
	};

	template <typename T>
	class property_sink_t : public data_sink_t
	{
		property_array_t<T> * m_property;
	   	std::vector<T> m_buffer;	
		T m_undefined_value;
	public:
		property_sink_t(property_array_t<T> & property, T undefined_value)
		{
			m_property = &property;
			m_undefined_value = undefined_value;
		}
		virtual void add_value(double value)
		{
			m_buffer.push_back(static_cast<T>(value));
		}
		virtual void flush()
		{
			m_property->assign(m_buffer, m_undefined_value);
		}
	};

	template <typename T>
	class vector_sink_t : public data_sink_t
	{
		std::vector<T> * m_vector;
	public:
		vector_sink_t(std::vector<cont_value_t> & vector)
		{m_vector = &vector;}
		virtual void add_value(double value)
		{
			m_vector->push_back(value);
		}
		virtual void flush()
		{}
	};
}

#endif // DATA_SINK_H_INCLUDED_ERTGHJKFGHUJIE4R5U689CVFGYH3456U3ERDFGFDFGHJJMHGRTYUISDFGPOIUYSAGHK765RTDF7TYRT9KLVB
