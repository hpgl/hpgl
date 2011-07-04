/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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
