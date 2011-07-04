/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____
#define __GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____

namespace hpgl
{
	bool gauss_solve(double * A, double * B, double * X, int size);
	bool cholesky_solve(double * A, double * B, double * X, int size);
}

#endif //__GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____