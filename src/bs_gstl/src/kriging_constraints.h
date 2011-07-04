/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __KRIGING_CONSTRAINTS_H__7015779C_655E_43F3_8394_1362F94DD6CB
#define __KRIGING_CONSTRAINTS_H__7015779C_655E_43F3_8394_1362F94DD6CB

#include "typedefs.h"

namespace hpgl
{
	

	template<kriging_constraint_kind_t kriging_kind>
	class kriging_constraints_t
	{};

	template<>
	class kriging_constraints_t<OK_CONSTRAINTS>
	{
	public:
		typedef OK_constraints constraints_t;
	};

	template<>
	class kriging_constraints_t<SK_CONSTRAINTS>
	{
	public:
		typedef SK_constraints constraints_t;
	};
}

#endif //__KRIGING_CONSTRAINTS_H__7015779C_655E_43F3_8394_1362F94DD6CB