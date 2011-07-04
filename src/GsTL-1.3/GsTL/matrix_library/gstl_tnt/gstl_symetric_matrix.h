#ifndef __GSTL_SYMETRIC_MATRIX_H__
#define __GSTL_SYMETRIC_MATRIX_H__


#include <GsTL/utils/gstl_error_messages.h>

#include <iostream>

namespace GSTL_TNT {


template <class T>
class Symmetric_matrix {

 public:

  typedef T element_type;
  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;

  Symmetric_matrix() {
    N_ =0; 
    size_ =0;
    val_ = 0;
    mat_=0;
  }

  Symmetric_matrix(int N);
  Symmetric_matrix(int N, reference val);

  ~Symmetric_matrix();

  Symmetric_matrix& resize(int N, int M);
  
  inline int num_rows() const;
  inline int num_cols() const;
  inline int dim() const {return N_;}
  inline reference operator()(int i, int j);
  inline const_reference operator()(int i, int j) const;
  
  // This is an awful function, but it can be useful
  inline T* raw_access() {return val_;}


 private:
  
  int N_;
  int size_;
  value_type* val_;
  value_type** mat_;

  void initialize(int N);
  void destroy();
};  // end class gstl_symetric_matrix





template<class T>
void Symmetric_matrix<T>::initialize(int N) {
  N_ = N;
  size_= (N*N + N) / 2;

  val_ = new T[size_];
  mat_ = new T*[N];

  for(int i=0; i<size_; i++)
    val_[i] = i;

  T* p = val_;
  for(int j=1; j<=N; j++) {
    mat_[j-1] = p;
    p += j;
  }
}


template<class T>
void Symmetric_matrix<T>::destroy() {
  if(val_ == 0 ) return ;

  delete [] mat_;
  delete [] val_;
}


template<class T>
Symmetric_matrix<T>::Symmetric_matrix(int N) {
  initialize(N);
}

template<class T>
Symmetric_matrix<T>::Symmetric_matrix(int N, reference val) {
  initialize(N);

  for(int i=0; i<size_; i++)
    val_[i] = val;
}


template<class T>
Symmetric_matrix<T>::~Symmetric_matrix() {
  destroy();
}


template<class T>
Symmetric_matrix<T>& Symmetric_matrix<T>::resize(int N, int) {
  if (N == N_)
    return *this;
  
  destroy();
  initialize(N);
  return *this;
}

template<class T>
inline int Symmetric_matrix<T>::num_rows() const {
  return N_;
}

template<class T>
inline int Symmetric_matrix<T>::num_cols() const {
  return N_;
}


template<class T>
inline T& Symmetric_matrix<T>::operator()(int i, int j) {
  gstl_assert(i>0 && i<=N_);
  gstl_assert(j>0 && j<=N_);
  
  if(i>j)
    return mat_[i-1][j-1];
  
  return mat_[j-1][i-1];
}

template<class T>
inline const T& Symmetric_matrix<T>::operator()(int i, int j) const {
  if(i>j)
    return mat_[i-1][j-1];
  
  return mat_[j-1][i-1];
}
  


/* ***************************  I/O  ********************************/

template <class T>
std::ostream& operator<<(std::ostream& s, const Symmetric_matrix<T>& A)
{
    int M=A.num_rows();

    s << M << " " << M << "\n";

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=M; j++)
        {
            s << A(i,j) << " ";
        }
        s << "\n";
    }

    return s;
}

}
#endif
