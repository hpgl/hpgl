/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498
#define __SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498

namespace hpgl
{
	template<typename source_t, typename indices_t, typename dest_t>
	inline void select(const source_t & src, const indices_t & indices, dest_t & dest)
	{
		dest.resize(indices.size());
		for (size_t i = 0, end_i = indices.size(); i < end_i; ++i)
		{
			dest[i] = src[indices[i]];
		}
	}
}

#endif //__SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498