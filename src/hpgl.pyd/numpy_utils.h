/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __NUMPY_UTILS_H__5A160939_4007_4FAE_94CD_277C1F492D31____
#define __NUMPY_UTILS_H__5A160939_4007_4FAE_94CD_277C1F492D31____

#include "hpgl_exception.h"

namespace hpgl
{
	inline bool check_axis_order(boost::python::object obj)
	{
		using namespace boost::python;
		bool is_f_contiguous = extract<bool>(obj.attr("flags")["F_CONTIGUOUS"]);
		return is_f_contiguous;
	}
  

	template<typename T, char kind>
	T * get_buffer_from_ndarray(boost::python::object obj, int size, const std::string & context)
	{
		using namespace std;
		using namespace boost;
		using namespace boost::python;

		if (!check_axis_order(obj))
			throw hpgl_exception(context, "Array is not F_CONTIGUOUS");

		string strkind = extract<string>(obj.attr("dtype").attr("kind"));
		
		if (strkind[0] != kind)
		{
			throw hpgl_exception(context, 
				format("Invalid dtype.kind: %s. Expected %s.") % strkind % kind);
		}

		int item_size_2 = extract<int>(obj.attr("itemsize"));

		if (item_size_2 != sizeof(T))
		{
			throw hpgl_exception(context,
					format("Invalid itemsize: %i. Expected %i.") % item_size_2 % sizeof(T));
		}
		
		object bufo = obj.attr("data");
		PyObject * buffer = bufo.ptr();
		PyBufferProcs * pb = buffer->ob_type->tp_as_buffer;
		
		void * result;
		int buf_size = (*pb->bf_getwritebuffer)(buffer, 0, &result);
		if (buf_size != size*sizeof(T))
		{
			throw hpgl_exception(context,
				format("Invalid buffer size: %i. Expected %i.") % buf_size % (size * sizeof(T)));
		}
		return (T*)result;
	}	

	inline int get_ndarray_len(boost::python::object obj)
	{
		using namespace boost::python;
		return (int) len(obj.attr("data")) / (int)extract<int>(obj.attr("itemsize"));
	}

	inline boost::shared_ptr<indicator_property_array_t> ind_prop_from_tuple(
		boost::python::tuple arr)
	{
		int size = get_ndarray_len(arr[0]);
		boost::shared_ptr<indicator_property_array_t> result
			(new indicator_property_array_t(
			get_buffer_from_ndarray<indicator_value_t, 'u'>(arr[0], size, "prop_from_tuple"),
			get_buffer_from_ndarray<unsigned char, 'u'>(arr[1], size, "prop_from_tuple"),
			size,
			boost::python::extract<int>(arr[2])));
		return result;
	}

	inline boost::shared_ptr<cont_property_array_t> cont_prop_from_tuple(
		boost::python::tuple arr)
	{
		int size = get_ndarray_len(arr[0]);
		boost::shared_ptr<cont_property_array_t> result
			(new cont_property_array_t(
			get_buffer_from_ndarray<cont_value_t, 'f'>(arr[0], size, "prop_from_tuple"),
			get_buffer_from_ndarray<unsigned char, 'u'>(arr[1], size, "prop_from_tuple"),
			size));
		return result;
	}
}

#endif //__NUMPY_UTILS_H__5A160939_4007_4FAE_94CD_277C1F492D31____
