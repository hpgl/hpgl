
#ifndef __GSTL_COVARIANCE_TABLE_H__
#define __GSTL_COVARIANCE_TABLE_H__

#include <GsTL/geometry/geometry.h>
#include <GsTL/utils/gstl_error_messages.h>



//=======================================
// Covariance table implementation

class Table_impl {
 public:

  typedef float value_type;

  Table_impl(int xsize, int ysize, int zsize) {
    table_ = new value_type[xsize*ysize*zsize];
    xy_ = new value_type*[ysize*zsize];
    xyz_ = new value_type**[zsize];
    
    value_type* current = table_;
    for(int i=0; i<ysize*zsize ; i++) {
      xy_[i]= current;
      current += xsize;
    }

    value_type** current2 = xy_;
    for(int j=0; j<zsize ; j++) {
      xyz_[j]= current2;
      current2 += ysize;
    }

    nx = xsize;
    ny = ysize;
    nz = zsize;
    nxy = nx*ny;
  }


  ~Table_impl() {
    delete [] xyz_;
    delete [] xy_;
    delete [] table_;
  }

  inline value_type& value_at(int x, int y, int z) {
    return xyz_[z][y][x];
  }

  inline const value_type& value_at(int x, int y, int z) const {
    return xyz_[z][y][x];
  }

  inline value_type& slow_access(int x, int y, int z) {
    return table_[z*nx*ny + y*nx +x];
  }

  inline value_type* raw_access() {
    return table_;
  }


 private:
  value_type* table_;
  value_type** xy_;
  value_type*** xyz_;

  int nx, ny, nz, nxy;

}; // end of class Table_impl





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
