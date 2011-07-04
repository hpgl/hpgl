/*
 *int kriging_solver( int neq, double *a, double *r, double *s ) return ising
 *----------------------------------------------------------------------
 *
 *               Solution of a System of Linear Equations
 *               ****************************************
 *
 *INPUT VARIABLES:
 *
 *  neq              number of equations
 *  * a              upper triangular left hand side matrix (stored
 *                   columnwise)
 *  * r              right hand side matrix (stored columnwise)
 *                   for okb2d, one column per variable
 *
 *
 *OUTPUT VARIABLES:
 *
 *  s()              solution array, same dimension as  r  above.
 *  ising            singularity indicator
 *                     0,  no singularity problem
 *                    -1,  neq .le. 1
 *                     k,  a null pivot appeared at the kth iteration
 *
 *PROGRAM NOTES:
 *
 *  1. Requires the upper triangular left hand side matrix.
 *  2. Pivots are on the diagonal.
 *  3. Does not search for max. element for pivot.
 *  4. Several right hand side matrices possible.
 *  5. USE for ok and sk only, NOT for UK.
 *
 *----------------------------------------------------------------------
*/

#include <algorithm>
#include <cmath>

static const double tol = 1.0e-7 ;

static void precondition(int neq, double *a) {
    if (neq < 2) {
        return;
    }
    double diag_sum = 0.;
    register int i;
    for (i =0; i < neq; i++) {
         int ii = (i*(i+1))*0.5 +i;
         diag_sum +=  a[ii];
    }
    diag_sum /= (double(neq)*1000.);
    diag_sum = std::min(0.0001, diag_sum);
    for (i =0; i < neq; i++) {
         int ii = (i*(i+1))*0.5 +i;
         a[ii] += diag_sum;
    }
}

static void precondition2(int neq, double *a) {
    if (neq < 2) {
        return;
    }
    double diag_sum = 0.;
    register int i;
    for (i =0; i < neq; i++) {
         int ii = i*neq +i;
         diag_sum +=  a[ii];
    }
    diag_sum /= (double(neq)*1000.);
    diag_sum = std::min(0.0001, diag_sum);
    for (i =0; i < neq; i++) {
         int ii = i*neq +i;
         a[ii] += diag_sum;
    }
}

/*!
 * Solution of a System of Linear Equations, where
 *  neq, number of equations
 *  a, upper triangular left hand side matrix (stored columnwise)
 *  r, right hand side matrix (stored columnwise)
 *  s, solution array, same dimension as  r  above.
 * Returns a singularity indicator equal to
 * 0,  no singularity problem, or
 * -1, if neq <= 1, or
 * k,  a null pivot appeared at the kth iteration.
 */
int kriging_solver( int neq, double *a, double *r, double *s ) {
     int nsb ;
     int nright = nsb = 1 ;
     int nm1, ii, ij, lp, ll, in, ll1, llb, i, j;
     double ak, piv, ap ;
     int ising = 0 ;

// If there is only one equation then set ising and return:

     if( neq <= 1 ) {
        ising = -1 ;
        s[0] = 1 ;
        return ising ;
        // gocad_error_message("Warning: neq <= 1", return ising;)
     }

// Initialize:
     precondition(neq, a);

     int nn = neq*(neq + 1) / 2 ;
     int nm = nsb*neq ;
     int m1 = neq - 1 ;
     int kk = 0 ;

// Start triangulation:

     for( int k=1; k<=m1; k++ ) {
        kk = kk+k ;
        ak = a[kk-1] ;
        if( fabs(ak) < tol) {
            ising = k ;
            //error_message("Error: k, a null pivot appeared at the kth iteration",  return ising;)
        }
        int km1 = k - 1 ;
        for( int iv=1; iv<=nright; iv++ ) {
           nm1 = nm*(iv-1) ;
           ii = kk + nn*(iv-1) ;
           piv = 1./a[ii-1] ;
           lp = 0 ;
           for( i=k; i<=m1; i++) {
              ll = ii ;
              ii = ii + i ;
              ap = a[ii-1]*piv ;
              lp = lp + 1 ;
              ij = ii - km1 ;
              for( j=i; j<=m1; j++) {
                 ij = ij + j ;
                 ll = ll + j ;
                 a[ij-1] = a[ij-1] - ap*a[ll-1] ;
              }
              for( llb=k; llb<=nm; llb+=neq) {
                 in = llb + lp + nm1 ;
                 ll1 = llb + nm1 ;
                 r[in-1] = r[in-1] - ap*r[ll1-1] ;
              }
           }
        }
     }

     int ijm = ij - nn*(nright - 1) ;
     if( fabs(a[ijm-1]) < tol) {
        ising = neq ;
     }

// Finished triangulation, start solving back:

     for( int iv=1; iv<=nright; iv++) {
        nm1 = nm*(iv - 1) ;
        ij = ijm + nn*(iv - 1) ;
        piv = 1./a[ij-1] ;
        for( llb=neq; llb<=nm; llb+=neq) {
           ll1 = llb + nm1 ;
           s[ll1-1] = r[ll1-1]*piv ;
        }
        i = neq ;
        kk = ij ;
        for( ii=1; ii<=m1; ii++) {
           kk = kk - i ;
           piv = 1./a[kk-1] ;
           i = i - 1 ;
           for( llb=i; llb<=nm; llb+=neq) {
              ll1 = llb + nm1 ;
              in = ll1 ;
              ap = r[in-1];
              ij = kk;
              for( j=i; j<=m1; j++) {
                 ij = ij + j ;
                 in = in + 1 ;
                 ap = ap - a[ij-1]*s[in-1] ;
              }
              s[ll1-1] = ap*piv ;
           }
        }
     }

// Finished solving back, return:

     return ising ;
}




template <class MaTRiX, class VecToR, class RandomIterator>
int gauss_solve(const MaTRiX &A, VecToR &b, RandomIterator solution) {
  int neq = A.num_rows();
  if(neq ==1 ) {
    *solution = b(1)/A(1,1);
    return 1;
  }

  int size = (neq*neq + neq)/2;
  assert(size > 0);
  assert(neq > 0);

  double* a = new double[size];
  double* r = new double[neq];
  double* s = new double[neq];
  
  int ind =0;
  for(int j=1; j<=neq; j++)
    for(int i=1; i<=j; i++) {
      assert(ind < size);
      a[ind++] = A(i,j);
    }
  assert(ind == size);

  for(int j=1; j<=neq; j++)
    r[j-1] = b(j);


  int ok = kriging_solver( neq, a, r, s );
  

  for(int j=0; j<neq; j++, solution++)
    *solution = s[j];

  delete [] a;
  delete [] r;
  delete [] s;

  return ok;

}
