#include <math.h>
#include <memory.h>
#include <assert.h>
#include "api.h"

double dot_row_col(double * m1, double * m2, int row, int col)
{
	double result = 0.0;
	for (int i = 0; i < 3; ++i)
	{
		result += m1[row * 3 + i] * m2[i * 3 + col];
	}
	return result;
}

void multiple_matrices(double * m1, double * m2, double * m3)
{
	double result[9];
	for (int row = 0; row < 3; ++row)
	{
		for (int col = 0; col < 3; ++col)
		{
			result[row * 3 + col] = dot_row_col(m1, m2, row, col);
		}
	}
	memcpy(m3, result, sizeof(result));
}

void cpycol(double * m, vector_t * dest, int col)
{
	assert(col < 3);
	assert(col >= 0);
	for (int i = 0; i < 3; ++i)
	{
		dest->m_data[i] = m[i*3 + col];
	}
}

void fill_ellipsoid_directions(ellipsoid_t * ell, double azimuth, double dip, double rotation)
{
	double coef = 3.14159265358 / 180.0;
	double razimuth = azimuth * coef;
	double rdip = dip * coef;
	double rrot = rotation * coef;

	double A[9] = {
		cos(razimuth), -sin(razimuth), 0,
		sin(razimuth), cos(razimuth), 0,
		0, 0, 1
	};

	double B[9] = {
		cos(rdip), 0, -sin(rdip),
		0, 1, 0,
		sin(rdip), 0, cos(rdip)
	};

	double C[9] = {
		1, 0, 0,
		0, cos(rrot), -sin(rrot),
		0, sin(rrot), cos(rrot)
	};

	double M[9] = {0};
	multiple_matrices(A, B, M);
	multiple_matrices(M, C, M);

	cpycol(M, &(ell->m_direction1), 0);
	cpycol(M, &(ell->m_direction2), 1);
	cpycol(M, &(ell->m_direction3), 2);
}

