#ifndef __GSTLAPPLI_GEOSTAT_ALGO_BLOCKCOVARIANCE_H__ 
#define __GSTLAPPLI_GEOSTAT_ALGO_BLOCKCOVARIANCE_H__ 

#include <GsTLAppli/geostat/common.h>

#include <GsTL/geometry/covariance.h>
#include <GsTLAppli/math/gstlvector.h>


template<class Location>
class Block_covariance : public Covariance<Location>{
 public:
  typedef Location first_argument_type;
  typedef Location second_argument_type;
  typedef typename Location::difference_type EuclideanVector;
  typedef typename Covariance_base<Location>::result_type result_type;

 public:
   Block_covariance():Covariance_base<Location>() {};
   ~Block_covariance(){};

   Block_covariance(const Block_covariance<Location>& rhs){
    cov_ = rhs.cov_;
    c0_ = rhs.c0_;
    block_vectors_ = rhs.block_vectors_;
   }

   Block_covariance(Covariance<Location> cov, float c0,
     std::vector< GsTLVector< float > > blk_vec )
     :cov_(cov),c0_(c0),block_vectors_(blk_vec) {};

  Block_covariance<Location>* clone() const {
    return new Block_covariance<Location>(this->cov_, this->c0_,this->block_vectors_);
  }

   Block_covariance(Covariance<Location>& cov, GsTLVector<int> block_pts, GsTLCoordVector cell_dim )
     :Covariance<Location>(), cov_(cov) { 
     // Find the set of vectors discretizing a block with center at (0,0,0) 
      GsTLVector< float> delta(cell_dim[0]/block_pts[0],
        cell_dim[1]/block_pts[1],cell_dim[2]/block_pts[2]);
      GsTLVector< float> start((-cell_dim[0] + delta[0])/2,
        (-cell_dim[1] + delta[1])/2,(-cell_dim[2] + delta[2])/2);
      n_blkpoints_ = block_pts[0]*block_pts[1]*block_pts[2];

      for(int i =0; i< block_pts[0]; i++ )
        for(int j=0; j<block_pts[1]; j++ )
          for(int k=0; k<block_pts[2]; k++ ){
            GsTLVector<float> vec(start[0] + i*delta[0], start[1] + j*delta[1],
              start[2] + k*delta[2]);
            block_vectors_.push_back(vec);
          }

      EuclideanVector vec_c0(0.,0.,0.);
      c0_ = compute(vec_c0);
   }

   result_type c0() const { return c0_;}


 // virtual result_type isotrop_compute( double h ) const;

  inline result_type operator()(const Location& u1, const Location& u2) const {
	return compute( u2 - u1 );
  }

  result_type  compute(const Location head, const Location tail) const {
	  return compute(tail-head); }

  result_type compute(const EuclideanVector& vec) const  {
    float cov = 0;
    std::vector< GsTLVector< float > >::const_iterator it = block_vectors_.begin();
    for(; it != block_vectors_.end() ; ++it) {
      EuclideanVector eVec(vec[0] + it->x(),vec[1] + it->y(),vec[2] + it->z());
      cov += cov_.compute(eVec);
  
     }
    return cov/n_blkpoints_;
  }


private:
  Covariance<Location> cov_;
  float c0_;
  std::vector< GsTLVector< float > > block_vectors_;
  float n_blkpoints_;

};

 /*  initialize(Covariance<Location>& cov, GsTLVector block_pts, GsTLCoordVector cell_dim )
     :Covariance_base<Location>(), cov_(cov) { 
     // Find the set of vectors discretizing a block with center at (0,0,0) 
      GsTLVector< float> delta(cell_dim[0]/block_pts[0],
        cell_dim[1]/block_pts[1],cell_dim[2]/block_pts[2]);
      GsTLVector< float> start((-cell_dim[0] + delta[0])/2,
        (-cell_dim[2] + delta[2])/2,(-cell_dim[2] + delta[2])/2);

     // float delta_x = cell_dim[0]/block_pts[0];
     // float delta_y = cell_dim[1]/block_pts[1];
     // float delta_z = cell_dim[2]/block_pts[2];

      for(int i =0; i< block_pts[0]; i++ )
        for(int j=0; j<block_pts[1]; j++ )
          for(int k=0; j<block_pts[2]; k++ ){
            GsTLVector<float> vec(start[0] + i*delta[0], start[1] + j*delta[1],
              start[2] + k*delta[2]);
            block_vectors_.push_back(vec);
          }
   }
*/


#endif
