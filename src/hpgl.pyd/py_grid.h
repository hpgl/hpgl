/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_GRID_H__C58F90E4_FD3F_46BE_9BC3_9C5E410AE3DE__
#define __PY_GRID_H__C58F90E4_FD3F_46BE_9BC3_9C5E410AE3DE__

#include <sugarbox_grid.h>

namespace hpgl
{
	class py_grid_t
	{
	public:
		py_grid_t();
		py_grid_t(
			sugarbox_grid_size_t x,
			sugarbox_grid_size_t y,
			sugarbox_grid_size_t z);
		sp_sugarbox_grid_t m_sugarbox_geometry;
	};

}

#endif //__PY_GRID_H__C58F90E4_FD3F_46BE_9BC3_9C5E410AE3DE__
