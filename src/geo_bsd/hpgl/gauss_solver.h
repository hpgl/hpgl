#ifndef __GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____
#define __GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____

namespace hpgl
{
	bool gauss_solve(double * A, double * B, double * X, int size);

	void cholesky_solve(double * A_L, double * A_U, double * B, double * X, int size);
	bool cholesky_decomposition(double * A, double * A_U, double * A_L, int size);

	bool cholesky_old(double * A, double * B, double * X, int size);
}

#endif //__GAUSS_SOLVER_H__2513A17A_DAF0_43DF_ADF0_BD80D8F29F38____