/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <lvm_data.h>

#include "py_lvm_data.h"
#include "py_mean_data.h"
#include "generate_complement_array.h"

namespace hpgl
{
	boost::shared_ptr<indicator_lvm_data_t> extract_lvm_data(boost::python::object & mean_data, int indicator_count)
	{
		boost::shared_ptr<indicator_lvm_data_t> lvm_data;
		boost::python::extract<py_indicator_lvm_data_t> e1(mean_data);
		boost::python::extract<boost::python::list> e2(mean_data);
		boost::python::extract<py_mean_data_t> e3(mean_data);

		if (e1.check())
		{
			py_indicator_lvm_data_t d = e1;
			lvm_data = d.m_lvm_data;
		} 
		if (e3.check())
		{	
			std::vector<boost::shared_ptr<std::vector<mean_t> > > lvms;
			py_mean_data_t plvm = e3;
			if (indicator_count == 1)
			{
				lvms.push_back(plvm.m_data);
			} else if (indicator_count == 2)
			{
				lvm_data.reset(new indicator_lvm_data_t());							
				boost::shared_ptr<std::vector<mean_t> > lvm0  = generate_complement_array(plvm.m_data);				
				lvms.push_back(lvm0);
				lvms.push_back(plvm.m_data);
			} else {
				throw hpgl_exception("extract_lvm_data", boost::format("Property has %s indicators, but only one mean array given.") % indicator_count);
			}
			lvm_data->assign(lvms);
		}
		else if (e2.check())
		{
			lvm_data.reset(new indicator_lvm_data_t());
			std::vector<boost::shared_ptr<std::vector<mean_t> > > lvms;

			boost::python::list l = e2;			
			int lvm_count = (int) boost::python::len(l);			
			
			if (lvm_count == 1 && indicator_count == 2)
			{
				py_mean_data_t plvm1 = boost::python::extract<py_mean_data_t>(l[0]);
				boost::shared_ptr<std::vector<mean_t> > lvm0  = generate_complement_array(plvm1.m_data);				
				lvms.push_back(lvm0);
				lvms.push_back(plvm1.m_data);				
			} 
			else if (indicator_count == lvm_count)
			{
				for (int i = 0; i < lvm_count; ++i)
				{
					py_mean_data_t plvm = boost::python::extract<py_mean_data_t>(l[0]);
					lvms.push_back(plvm.m_data);
				}
			} 
			else 
			{
				throw hpgl_exception("hpgl::py_sis_lvm", boost::format("Property has %s indicators, but %s mean arrays given.") % indicator_count % lvm_count);
			}
			lvm_data->assign(lvms);			
		}
		return lvm_data;
	}
}