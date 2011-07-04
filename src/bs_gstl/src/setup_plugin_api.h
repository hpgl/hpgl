/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "setup_common_api.h"

#ifdef BS_API_PLUGIN 
	#undef BS_API_PLUGIN
#endif
#ifdef BS_C_API_PLUGIN 
	#undef BS_C_API_PLUGIN
#endif
#ifdef BS_HIDDEN_API_PLUGIN 
	#undef BS_HIDDEN_API_PLUGIN
#endif

//setup plugins API macro
#ifdef BS_EXPORTING_PLUGIN
	#define BS_API_PLUGIN _BS_API_EXPORT
	#define BS_C_API_PLUGIN _BS_C_API_EXPORT
	#define BS_HIDDEN_API_PLUGIN _BS_HIDDEN_API_EXPORT
#else
	#define BS_API_PLUGIN _BS_API_IMPORT
	#define BS_C_API_PLUGIN _BS_C_API_IMPORT
	#define BS_HIDDEN_API_PLUGIN _BS_HIDDEN_API_IMPORT
#endif

