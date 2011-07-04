#ifndef __GSTL_SYMETRIC_MATRIX_H__
#define __GSTL_SYMETRIC_MATRIX_H__


#include <cassert>


template <class T>
class Symetric_matrix {

 public:

  typedef T value_type;
  typedef T& reference;
  typedef const T& const_reference;

  Symetric_matrix(int N);
  Symetric_matrix(int N, reference val);

  ~Symetric_matrix();

  Symetric_matrix& resize(int N);
  
  inline int num_rows() const;
  inline int num_cols() const;

  inline reference operator()(int i, int j);
  inline const_reference operator()(int i, int j) const;
  

 private:
  
  int N_;
  int size_;
  value_type* val_;
  value_type** mat_;

  void initialize(int N);
  void destroy();
};  // end class gstl_symetric_matrix





template<class T>
void Symetric_matrix<T>::initialize(int N) {
  N_ = N;
  size_= (N*N - N) / 2;

  val_ = new T[size_];
  mat_ = new T*[N];

  T* p = val_;
  for(int i=1; i<=N; i++) {
    mat_[i] = p;
    p += i;
  }
}


template<class T>
void Symetric_matrix<T>::destroy() {
  delete [] mat_;
  delete [] val_;
}


template<class T>
Symetric_matrix<T>::Symetric_matrix(int N) {
  initialize(N);
}

template<class T>
Symetric_matrix<T>::Symetric_matrix(int N, reference val) {
  initialize(N);

  for(int i=0; i<size_; i++)
    val_[i] = val;
}


template<class T>
Symetric_matrix<T>::~Symetric_matrix() {
  destroy();
}


template<class T>
Symetric_matrix<T>& Symetric_matrix<T>::resize(int N) {
  if (N == N_)
    return *this;

  destroy();
  initialize(N);
}

template<class T>
inline int Symetric_matrix<T>::num_rows() const {
  return N_;
}

template<class T>
inline int Symetric_matrix<T>::num_cols() const {
  return N_;
}


template<class T>
inline T& Symetric_matrix<T>::operator()(int i, int j) {
  if(i>j)
    return mat_[i-1][j-1];
  
  return mat_[j-1][i-1];
}

template<class T>
inline const T& Symetric_matrix<T>::operator()(int i, int j) const {
  if(i>j)
    return mat_[i-1][j-1];
  
  return mat_[j-1][i-1];
}
  

#endif
