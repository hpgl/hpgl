/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __GEOMETRY_SERVER_FWD_H__A2740E45_CACA_4CD1_BF00_D61403770357__
#define __GEOMETRY_SERVER_FWD_H__A2740E45_CACA_4CD1_BF00_D61403770357__

namespace hpgl
{
	class geometry_server_t;
	typedef smart_ptr<geometry_server_t, false> sp_geometry_server_t;

	class sugarbox_geometry_server_t;
	typedef smart_ptr<sugarbox_geometry_server_t, false> sp_sugarbox_geometry_server_t;
}

#endif //__GEOMETRY_SERVER_FWD_H__A2740E45_CACA_4CD1_BF00_D61403770357__
