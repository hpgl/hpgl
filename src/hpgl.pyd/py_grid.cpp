/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "py_grid.h"

namespace hpgl
{
py_grid_t::py_grid_t()
{}

py_grid_t::py_grid_t(
		sugarbox_grid_size_t x,
		sugarbox_grid_size_t y,
		sugarbox_grid_size_t z)
{
	m_sugarbox_geometry = boost::shared_ptr<sugarbox_grid_t>(new sugarbox_grid_t());
	m_sugarbox_geometry->init(x, y, z);
}
}
