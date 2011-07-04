/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB
#define __PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB

#include "lvm_data.h"

namespace hpgl
{
	class py_indicator_lvm_data_t
	{
	public:
		boost::shared_ptr<indicator_lvm_data_t> m_lvm_data;
	};

	py_indicator_lvm_data_t py_load_indicator_mean_data(PyObject * filenames);
}

#endif //__PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB