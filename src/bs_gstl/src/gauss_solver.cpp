/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#include "stdafx.h"

namespace hpgl
{
	bool gauss_solve(double * A, double * B, double * X, int size)
	{
		std::vector<int> flags(size, 0);
		std::vector<int> order(size, 0);
		for (int i = 0; i < size; ++i)
		{
			//searching for non zero row;
			bool found = false;
			int row = -1;
			for (int j = 0; j < size; ++j)
			{
				if (flags[j] == 0 && A[j * size + i] != 0)
				{
					found = true;
					row = j;
					flags[j] = 1;
					order[i] = j;
					break;
				}
			}

			if (!found)
			{
				return false; //matrix is singular
			}

			//normalize row
			double coef = A[row * size + i];
			for (int j = i; j < size; ++j)
			{
				A[row * size + j] /= coef;
			}
			B[row] /= coef;

			//subtract row
			for (int j = 0; j < size; ++j)
			{
				if (flags[j] == 0)
				{
					double coef =  A[j * size + i];
					for (int k = i; k < size; ++k)
					{
						A[j * size + k] -= coef * A[row * size + k];
					}
					B[j] -= coef * B[row];
				}
			}
		}

		for (int i = size-1; i >=0 ; --i)
		{
			int row = order[i];
			X[i] = B[i];
			for (int j = size-1; j >i; --j)
			{
				X[i] -= A[row * size + j] * X[j];
			}	
		}

		return true;
	}
}