/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_SIS_PARAMS_H__BD1564B9_4612_4F90_B776_BD7EA16FFDC8
#define __PY_SIS_PARAMS_H__BD1564B9_4612_4F90_B776_BD7EA16FFDC8

#include "ik_params.h"

namespace hpgl
{	
	void parse_sis_params(PyObject * params, ik_params_t & ik_params);
}

#endif //__PY_SIS_PARAMS_H__BD1564B9_4612_4F90_B776_BD7EA16FFDC8