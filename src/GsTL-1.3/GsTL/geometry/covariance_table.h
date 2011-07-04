
#ifndef __GSTL_COVARIANCE_TABLE_H__
#define __GSTL_COVARIANCE_TABLE_H__

#include <GsTL/utils/gstl_error_messages.h>
#include <GsTL/geometry/geometry.h>


//=======================================
// Covariance table

class Covariance_table {
 public:

  template<class Covariance>
  Covariance_table(int nx, int ny, int nz, 
		   const Covariance& covar) 
    : nx_(nx), ny_(ny), nz_(nz), 
    xsize_(nx*2+1), ysize_(ny*2+1), zsize_(nz*2+1), xysize_(xsize_*ysize_),
    size_(xsize_*ysize_*zsize_)  {
    
    covar_table_ = new float[size_];
    
    location_3d<int> center(0,0,0);
    
    int i=0;

    for(int z = -nz ; z <= nz ; z++)
    for(int y = -ny ; y <= ny ; y++)
      for(int x = -nx ; x <= nx ; x++) {
	location_3d<int> loc(x,y,z);
	covar_table_[i++] = static_cast<float>( covar(center, loc) );
      }
  }

  /** Copy constructor. 
   * Since a covariance table is probably a very large object, it is certainly 
   * not a good idea to use the copy constructor, at least not with the current
   * implementation. A smarter implementation would use smart pointers and only
   * copy the pointer to the table instead of the deep copy currently done.
   */
  Covariance_table(const Covariance_table& rhs) {
    covar_table_ = new float[rhs.size_];
    for(int i=0; i<rhs.size_; i++)
      covar_table_[i] = rhs.covar_table_[i];

    nx_ = rhs.nx_;
    ny_ = rhs.ny_;
    nz_ = rhs.nz_;
    xsize_ = rhs.xsize_;
    ysize_ = rhs.ysize_;
    zsize_ = rhs.zsize_;
    xysize_ = rhs.xysize_;
    size_ = rhs.size_;
  }

  /** Assignment. 
   * Since a covariance table is probably a very large object, it is certainly 
   * not a good idea to use the assignment operator, at least not with the current
   * implementation. A smarter implementation would use smart pointers and only
   * copy the pointer to the table instead of the deep copy currently done.
   */
  Covariance_table& operator=(const Covariance_table& rhs) {
    if(covar_table_ != 0)
      delete [] covar_table_;

    covar_table_ = new float[rhs.size_];
    for(int i=0; i<rhs.size_; i++)
      covar_table_[i] = rhs.covar_table_[i];

    nx_ = rhs.nx_;
    ny_ = rhs.ny_;
    nz_ = rhs.nz_;
    xsize_ = rhs.xsize_;
    ysize_ = rhs.ysize_;
    zsize_ = rhs.zsize_;
    xysize_ = rhs.xysize_;
    size_ = rhs.size_;
    
    return *this;
  }


  ~Covariance_table() {
    delete [] covar_table_ ;
  }
  
  template <class Location>
  inline double operator()(const Location& u1, const Location& u2) const;


 private:

  float* covar_table_;

  int nx_, ny_, nz_;
  int xsize_, ysize_, zsize_;
  int xysize_;
  int size_;

};  // end of class Covariance_table



template <class Location>
inline double 
Covariance_table::operator()(const Location& u1, const Location& u2) const {
  typedef typename Location::difference_type Euclidean_vector;
  Euclidean_vector vec = u2-u1;
  int index = (static_cast<int>(vec[2])+nz_)*xysize_  + 
              (static_cast<int>(vec[1])+ny_)*xsize_  + 
              static_cast<int>(vec[0])+nx_;

  gstl_assert(index >=0);
  if(index > size_)
    return 0;
  else
    return covar_table_[index];
}



#endif
