/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __IS_INFORMED_PREDICATE_H__886B4621_D084_4CA4_B800_69AD4C7CD215
#define __IS_INFORMED_PREDICATE_H__886B4621_D084_4CA4_B800_69AD4C7CD215

namespace hpgl
{
	template<typename prop_t>
	class is_informed_predicate_t
	{
		const prop_t & m_prop;
	public:
		is_informed_predicate_t(const prop_t & prop): m_prop(prop)
		{}

		bool operator()(node_index_t neighbour)const
		{
			return m_prop.is_informed(neighbour);
		}
	};
}

#endif //__IS_INFORMED_PREDICATE_H__886B4621_D084_4CA4_B800_69AD4C7CD215