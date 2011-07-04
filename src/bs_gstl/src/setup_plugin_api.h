/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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

