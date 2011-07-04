/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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
