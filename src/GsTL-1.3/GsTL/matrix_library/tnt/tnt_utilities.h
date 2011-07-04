#ifndef TNT_UTILITIES_H
#define TNT_UTILITIES_H


#include "cmat.h"
#include "subscript.h"
#include "vec.h"
#include "index.h"
 
namespace TNT
{

  /** Concatenates two matrices with the same number of rows.
   * if A is M x N1 and B is M x N2,
   * C=concatenate_row(A,B) is M x (N1+N2)
   */
  template <class T>
    inline Matrix<T> concatenate_row( Matrix<T> &A, 
				      Matrix<T> &B){
 
    Subscript M = A.num_rows();
    Subscript N1 = A.num_cols();
    
    assert(B.num_rows()==M);
    Subscript N2 = B.num_cols();
    
    Matrix<T> tmp(M,N1+N2);
    
    Index1D I(1,M);
    Index1D J1(1,N1);
    Index1D J2(1,N2);
    Index1D J3(N1+1,N1+N2);
    
    tmp(I,J1)=A(I,J1);
    tmp(I,J3)=B(I,J2);
    
    return tmp;
  }



  template<class T>
    inline Matrix<T> concatenate_col( Matrix<T> &A, 
				      Matrix<T> &B){
    
    Subscript M1 = A.num_rows();
    Subscript N = A.num_cols();
    
    assert(B.num_cols()==N);
    Subscript M2 = B.num_rows();
    
    Matrix<T> tmp(M1+M2,N);
    
    Index1D I1(1,M1);
    Index1D I2(1,M2);
    Index1D I3(M1+1,M1+M2);
    Index1D J(1,N);

    tmp(I1,J)=A(I1,J);
    tmp(I3,J)=B(I2,J);
    
    return tmp;
  }
  

  template <class T>
    inline Vector<T> concatenate_vect( Vector<T> &A, 
				      Vector<T> &B){
 
    Subscript N1 = A.dim();
    
    Subscript N2 = B.dim();
    
    Vector<T> tmp(N1+N2);
    
    for(int i=1;i<=N1;i++)
      tmp(i)=A(i);

    int j=1;
    for(int i=N1+1;i<=N1+N2;i++)
      tmp(i)=A(j++);
      
    return tmp;
  }



  
}


#endif

