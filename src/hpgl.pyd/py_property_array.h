/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_PROPERTY_ARRAY_H__60AF2D27_9765_4844_9E4A_C32AD4B775FD__
#define __PY_PROPERTY_ARRAY_H__60AF2D27_9765_4844_9E4A_C32AD4B775FD__

#include <property_array.h>
#include <hpgl_exception.h>

namespace hpgl
{
	typedef std::vector<float> float_vector_t;

	class py_cont_property_array_t
	{
	public:
		sp_double_property_array_t m_double_property_array;

		py_cont_property_array_t(){};

		py_cont_property_array_t(int size)
		{
			m_double_property_array.reset(new cont_property_array_t(size));
		}

		~py_cont_property_array_t()
		{
		}
		int size();
		bool is_informed(int index)const;
		cont_value_t get_at(int index)const;
		void set_at(int index, double value);
		py_cont_property_array_t clone()const
		{
			py_cont_property_array_t result;
			result.m_double_property_array = m_double_property_array->clone();
			return result;
		}

		float * get_raw_float_pointer()
		{
			return const_cast<float *>(&(m_double_property_array->data()[0]));
		}

		float_vector_t * get_raw_float_vector_pointer()
		{
			return const_cast<float_vector_t *>(&(m_double_property_array->data()));
		}
	};


	typedef std::vector<unsigned char> byte_vector_t;

	class py_byte_property_array_t
	{
	public:
		sp_byte_property_array_t m_byte_property_array;

		py_byte_property_array_t(){};

		py_byte_property_array_t(int size, boost::python::object values)
		{
			m_byte_property_array.reset(new indicator_property_array_t(size));
			int icount = (int) boost::python::len(values);
			if (icount < 0)
				throw hpgl_exception("py_byte_property_array_t", boost::format("Too many values: %s. (Overflow?)") % icount);
			set_indicator_count(*m_byte_property_array, icount);
		}

		~py_byte_property_array_t()
		{}

		int size()const;
		bool is_informed(int index)const;
		indicator_value_t get_at(int index)const;
		void set_at(int index, int value);
		py_byte_property_array_t clone()const
		{
			py_byte_property_array_t result;
			result.m_byte_property_array = m_byte_property_array->clone();
			return result;
		}

		unsigned char * get_raw_byte_pointer()
		{
			return const_cast<unsigned char*>(&(m_byte_property_array->data()[0]));
		}

		byte_vector_t * get_raw_byte_vector_pointer()
		{
			return const_cast<byte_vector_t *>(&(m_byte_property_array->data()));
		}
	};
}


// deprecated

/*
namespace hpgl
{
	class py_property_array_t
	{
	public:
		sp_double_property_array_t m_double_property_array;
		sp_byte_property_array_t m_byte_property_array;
		~py_property_array_t()
		{
		}

		int size()
		{
			if(m_double_property_array->size()==0)
				if(m_byte_property_array->size()==0)
					return 0;
				else
					return m_byte_property_array->size();
			else
				return m_double_property_array->size();
	};
}
*/
#endif //__PY_PROPERTY_ARRAY_H__60AF2D27_9765_4844_9E4A_C32AD4B775FD__
