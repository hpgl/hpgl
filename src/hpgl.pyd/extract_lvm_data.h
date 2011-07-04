/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A
#define __EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A

namespace hpgl
{
	boost::shared_ptr<indicator_lvm_data_t> extract_lvm_data(boost::python::object & mean_data, int indicator_count);
}

#endif //__EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A