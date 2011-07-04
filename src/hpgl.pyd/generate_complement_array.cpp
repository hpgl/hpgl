/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

#include <typedefs.h>

namespace hpgl
{

	boost::shared_ptr<std::vector<mean_t> > generate_complement_array(boost::shared_ptr<std::vector<mean_t> > in)
	{
		boost::shared_ptr<std::vector<mean_t> > lvm0 (new std::vector<mean_t>());
		lvm0->reserve(in->size());
		for (size_t idx = 0, end_idx = in->size(); idx < end_idx; ++idx)
		{
			lvm0->push_back(1 - in->operator[](idx));
		}
		return lvm0;
	}

}