/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef HPGL_TRACE_FUNCTIONS_H_INCLUDED_DALSJFAASDKFKJWEHKRNKCKWEUNEKRNWKENCF
#define HPGL_TRACE_FUNCTIONS_H_INCLUDED_DALSJFAASDKFKJWEHKRNKCKWEUNEKRNWKENCF

#ifdef HPGL_TRACE_DEBUG
#define HPGL_DEBUG(x) std::cout << "[DEBUG] " << x
#define HPGL_TRACE_WARNING
#else
#define HPGL_DEBUG(x)
#endif

#ifdef HPGL_TRACE_WARNING
#define HPGL_WARNING(x) std::cout << "[WARNING] " << x
#define HPGL_TRACE_ERROR
#else
#define HPGL_WARNING(x)
#endif

#ifdef HPGL_TRACE_ERROR
#define HPGL_ERROR(x) std::cout << "[ERROR] " << x
#else
#define HPGL_ERROR(x)
#endif

#endif
