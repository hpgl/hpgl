/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

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