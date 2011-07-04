/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7
#define __COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7

namespace hpgl
{
	class covariance_model_t
	{
	public:
		virtual covariance_t operator()(coord_t, coord_t)const = 0;
	};
}

#endif //__COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7