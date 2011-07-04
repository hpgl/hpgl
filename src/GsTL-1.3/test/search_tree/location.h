#ifndef __GTL_LOCATION_EXAMPLE__
#define __GTL_LOCATION_EXAMPLE__


struct location_3d{
  location_3d(int x=0, int y=0, int z=0){
    coord_[0]=x; coord_[1]=y; coord_[2]=z;
  }

  inline bool operator==(location_3d loc) {
    if( loc[0]==coord_[0] && loc[1]==coord_[1] && loc[2]==coord_[2] )
      return true;
    else
      return false;
  }

  inline int& operator[](unsigned int i) {return coord_[i];}
  inline const int& operator[](unsigned int i) const {return coord_[i];}
  
private:
  int coord_[3];
};


struct location_2d{
  location_2d(int x=0, int y=0){
    coord_[0]=x; coord_[1]=y;
  }

  inline bool operator==(location_2d loc) {
    if( loc[0]==coord_[0] && loc[1]==coord_[1] )
      return true;
    else
      return false;
  }

  inline int& operator[](unsigned int i) {return coord_[i];}
  inline const int& operator[](unsigned int i) const {return coord_[i];}
  
private:
  int coord_[2];
};


#endif
