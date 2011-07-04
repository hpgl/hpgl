#ifndef __GTL_CARTESIAN_GRID_EXAMPLE__
#define __GTL_CARTESIAN_GRID_EXAMPLE__


#include "location.h"

#include <cmath>
#include <cassert>
#include <cstdlib>

#include <vector>
#include <iterator>

class geovalue;


class cartesian_grid{
 
 private:

  std::vector<int> values_;
  int nrow_, ncol_;
  int outside_val_;

  inline int hash(location_2d loc);
  inline int hash(int i, int j);
  inline location_2d dehash(int index);



 public:

  cartesian_grid(int nrow=1, int ncol=1):nrow_(nrow),ncol_(ncol),outside_val_(-99) {values_.resize(nrow*ncol);}

  void random_generate(){
    for(unsigned int i=0; i<values_.size(); i++){
      double val=drand48();
      if(val<0.5)
	values_[i]=0;
      else
	values_[i]=1;
    }
  }

  void initialize(int* values) {
    for(int i=0; i<nrow_*ncol_ ; i++)
      values_[i] = values[i];
  }

 int size() {return values_.size();}
  
 geovalue operator()(int i, int j);

 geovalue operator()(int index);

 inline location_2d location_of(int index) {return dehash(index);}
 inline int& prop_value_at(int index); 
 const int& prop_value_at(int index) const {return values_[index];}

  //---------------------
  class iterator {
  private:
    int index_;
    cartesian_grid& grid_;
 
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef geovalue                  value_type;
    typedef int                       difference_type;
    typedef geovalue*                 pointer;
    typedef geovalue&                 reference;

  public:
    iterator(cartesian_grid& grid, int index):index_(index),grid_(grid){};
    inline iterator& operator++(int){index_ ++; return *this;}
    inline iterator operator++(){index_ ++; return *this;}
    inline bool operator!=(iterator it){return index_ != it.index_;}
    geovalue operator*();
    geovalue* operator->();
    inline int operator-(iterator it) {return index_ - it.index_;}
  };
  //---------------------

  iterator begin(){iterator it(*this, 0); return it;}
  iterator end() {iterator it(*this, nrow_*ncol_); return it;}
};




inline int& cartesian_grid::prop_value_at(int index){
  if(index < int(values_.size()) )
    return values_[index];
  else 
    return outside_val_;
}

inline int cartesian_grid::hash(location_2d loc){
  int res;
  if( (loc[0]<0 || loc[0] >=nrow_) ||
      (loc[1]<0 || loc[1] >=ncol_ ) )
      res =  -9;
  else
    res = loc[0]*ncol_+loc[1];
  return res;
}

inline int cartesian_grid::hash(int i, int j){
  int res;
  if( (i<0 || i >=nrow_) ||
	(j<0 || j >=ncol_ ) )
    res =  -9;
  else
    res = i*ncol_+j;
  return res;
}

inline location_2d cartesian_grid::dehash(int index){
  location_2d loc;
  loc[0]=int(floor(double(index)/double(ncol_)));
  loc[1]=index%ncol_;
  return loc;
}



#endif
