/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __VAR_RADIX_UTILS_H__89D51E7C_755F_44CE_97B4_2D086B4EAEC3
#define __VAR_RADIX_UTILS_H__89D51E7C_755F_44CE_97B4_2D086B4EAEC3

namespace hpgl
{
	template<typename T>
	inline T vr_to_dec(int r2, int r1, T d3, T d2, T d1)
	{
		return d3 * r2 * r1 + d2 * r1 + d1;
	}

	template<typename T>
	inline void dec_to_vr(int r2, int r1, T dec, T & d3, T & d2, T & d1)
	{
		d1 = dec % r1;
		d2 = (dec % (r2 * r1) ) / r1;
		d3 = dec / (r2 * r1);
	}
}

#endif //__VAR_RADIX_UTILS_H__89D51E7C_755F_44CE_97B4_2D086B4EAEC3