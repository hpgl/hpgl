/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __GET_COMPILE_TIME_H__AD399D78_C5A0_42E2_80E2_F862D9E16612__
#define __GET_COMPILE_TIME_H__AD399D78_C5A0_42E2_80E2_F862D9E16612__

std::string get_compile_time()
{
	std::string result = __TIME__;
	for (size_t i = 0; i < result.size(); ++i)
		if (result[i] == ':')
			result[i] = '_';
	return result;
}

#endif //__GET_COMPILE_TIME_H__AD399D78_C5A0_42E2_80E2_F862D9E16612__
