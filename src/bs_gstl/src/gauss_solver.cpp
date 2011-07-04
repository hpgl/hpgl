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

	bool cholesky_decomposition(double * A, double * A_U, double * A_L, int size)
	{
		double V = 0.0;
		
		// inside matrix [L(i,j)]
		for (int j = 0; j < size; j++)
		{
			for(int i = j; i < size; i++)
			{
				if(i==j)
				{
					// main diagonals [L(i,i)]
					//for (int i = 0; i < size; i++)
					//{
						V = 0.0;
						V += A[i*size + i];
						for (int k = 0; k <= i-1; k++)
						{
							V -= (A_U[k*size + i] * A_U[k * size + i]);
						}
						
						if( V <= 0)
						{
							return false;
						}

						A_L[i*size + i] = sqrt(V);
						A_U[i*size + i] = sqrt(V);
					//}
				}
				else
				{
						V = 0.0;
						for (int k = 0; k <= j-1; k++)
						{
							V += A_U[k*size + i] * A_U[k*size + j];
						}

						if( A_U[j*size + j] == 0)
						{
							return false;
						}
						
						A_U[j*size + i] = (1 / A_U[j*size + j]) * (A[j*size + i] - V);
						A_L[i*size + j] = A_U[j*size + i];
				}
			}
		}
		return true;
	}

	void cholesky_solve(double * A_L, double * A_U, double * B, double * X, int size)
	{
		// A[j * size + i]
		// B[i]
		// X[i]

		std::vector<double> X_R(size,0.0);

//		gauss_solve(&A_L[0], &B[0], &X_R[0], size);

		for (int i = 0; i <size ; i++)
		{
			X_R[i] = B[i];
			for (int j = 0; j <i; j++)
			{
				X_R[i] -= A_L[i * size + j] * X_R[j];
			}	
			X_R[i] /= A_L[i * size + i];
		}

//	  gauss_solve(&A_U[0], &X_R[0], X, size);
		
		for (int i = size-1; i >=0 ; --i)
		{
			X[i] = X_R[i];
			for (int j = size-1; j >i; --j)
			{
				X[i] -= A_U[i * size + j] * X[j];
			}	
			X[i] /= A_U[i * size + i];
		}
    }

	bool cholesky_old(double * A, double * B, double * X, int size)
	{
		double V = 0.0;

		std::vector<double> A_U(size*size,0.0);
		std::vector<double> A_L(size*size,0.0);
		
		// inside matrix [L(i,j)]
		for (int j = 0; j < size; j++)
		{
			for(int i = j; i < size; i++)
			{
				if(i==j)
				{
					// main diagonals [L(i,i)]
					//for (int i = 0; i < size; i++)
					//{
						V = 0.0;
						V += A[i*size + i];
						for (int k = 0; k <= i-1; k++)
						{
							V -= (A_U[k*size + i] * A_U[k * size + i]);
						}
						
						if( V <= 0)
						{
							return false;
						}

						A_L[i*size + i] = sqrt(V);
						A_U[i*size + i] = sqrt(V);
					//}
				}
				else
				{
						V = 0.0;
						for (int k = 0; k <= j-1; k++)
						{
							V += A_U[k*size + i] * A_U[k*size + j];
						}

						if( A_U[j*size + j] == 0)
						{
							return false;
						}
						
						A_U[j*size + i] = (1 / A_U[j*size + j]) * (A[j*size + i] - V);
						A_L[i*size + j] = A_U[j*size + i];
				}
			}
		}

		std::vector<double> X_R(size,0.0);

//		gauss_solve(&A_L[0], &B[0], &X_R[0], size);

		for (int i = 0; i <size ; i++)
		{
			X_R[i] = B[i];
			for (int j = 0; j <i; j++)
			{
				X_R[i] -= A_L[i * size + j] * X_R[j];
			}	
			X_R[i] /= A_L[i * size + i];
		}

//	  gauss_solve(&A_U[0], &X_R[0], X, size);
		
		for (int i = size-1; i >=0 ; --i)
		{
			X[i] = X_R[i];
			for (int j = size-1; j >i; --j)
			{
				X[i] -= A_U[i * size + j] * X[j];
			}	
			X[i] /= A_U[i * size + i];
		}
    }


	}
