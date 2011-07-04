#ifndef __LU_SIMULATOR__
#define __LU_SIMULATOR__

#include <GsTL/matrix_library/tnt/cmat.h>
#include <GsTL/matrix_library/tnt/cholesky.h>
#include <GsTL/matrix_library/tnt/trisolve.h>
#include <GsTL/matrix_library/tnt/gstl_gauss_solver.h>
#include <GsTL/kriging/helper_functions.h>
#include <GsTL/matrix_library/tnt/lu.h>

#include <iostream>


template<
  typename Covariance,
  typename Rand_generator,
  typename Grid
>
class LU_simulator {

public :
  LU_simulator(Covariance covar, Rand_generator gen, Grid* grid)
    :covar_(covar),gen_(gen), grid_(grid){};
  ~LU_simulator(){};

  template<
    typename GvalIterator
  >
  void initialize_matrix(GvalIterator unk_begin, GvalIterator unk_end,
       GvalIterator data_begin , GvalIterator data_end)    
  {
    size_u_ = std::distance(unk_begin,unk_end);
    size_d_ = std::distance(data_begin,data_end);
    if(size_u_ == 0 ) return ;
    TNT::Matrix< float > C(size_u_+size_d_,size_u_+size_d_);
    w_.resize(size_u_+size_d_);
    L_.resize(size_u_+size_d_,size_u_+size_d_);

    TNT::Vector< float > z_(size_u_+size_d_);

    build_C(  C,1,1,data_begin, data_end,data_begin, data_end, covar_  );
    build_C(  C,size_d_+1,1,unk_begin, unk_end,data_begin, data_end, covar_  );
    build_C(  C,1,size_d_+1,data_begin, data_end,unk_begin, unk_end, covar_  );
    build_C(  C,size_d_+1,size_d_+1,unk_begin, unk_end,unk_begin, unk_end, covar_  );
    
    TNT::Cholesky_upper_factorization(C,L_);

    if( size_d_ > 0 ) {
      //Build a vector with the data
      TNT::Vector< float > z(size_d_);
      int ind=1;
      for(GvalIterator it = data_begin; it != data_end; ++it, ++ind ) 
        z(ind) = it->property_value();
  
      TNT::Matrix< float > Ldd(size_d_,size_d_);
      for(int i=1;i<=size_d_;i++)
        for(int j=1;j<=i;j++)
          Ldd(i,j)=L_(i,j);
      TNT::Vector< float > wd  = Lower_triangular_solve(Ldd, z);
      
      for(int i=1;i<=size_d_;i++) w_(i) = wd(i);
    }
  }

  template<
    typename GvalIterator,
    typename MarginalCdf
  >
  int operator()(GvalIterator unk_begin, GvalIterator unk_end,
  MarginalCdf marginal) {

    if( std::distance(unk_begin,unk_end) != size_u_ ) return 1;
    TNT::Vector< float > z(size_u_);
    //if( size_d_ > 0 ) 
    for(int i=size_d_+1;i<=size_d_+size_u_;i++) w_(i) = marginal.inverse( gen_() );

    for(int row=size_d_+1; row<=size_d_+size_u_;row++) {
      float val=0;
      for(int col=1; col<=row; col++) {
        val += L_(row,col)*w_(col);
      }
      z(row-size_d_)=val;
    }
    //z = L_*w_;
    int ind=1;
    for( ; unk_begin != unk_end; ++unk_begin, ind++ ) {
     // unk_begin->set_property_array(grid_->selected_property());
      unk_begin->set_property_value(z(ind));
    }

  }


  template<
    typename GvalIterator,
    typename MarginalCdf
  >
  int operator()(GvalIterator unk_begin, GvalIterator unk_end,
       GvalIterator data_begin , GvalIterator data_end, MarginalCdf marginal)
  {
    //Compute the Lu simulation with
    //  z_u = C_ud * inv(C_dd)*z_d + L_uu * w
    // where d is data and u is unknown
    // L_uu is the L of the Cholesky decomposition of C_uu - C_ud * inv(C_dd) * C_ud'

    int size_u = std::distance(unk_begin,unk_end);
    int size_d = std::distance(data_begin,data_end);
    if(size_u == 0 ) return 1;
    TNT::Matrix< float > C(size_u+size_d,size_u+size_d);
    TNT::Matrix< float > L(size_u+size_d,size_u+size_d);
    TNT::Vector< float > z(size_u+size_d);

    build_C(  C,1,1,data_begin, data_end,data_begin, data_end, covar_  );
    build_C(  C,size_d+1,1,unk_begin, unk_end,data_begin, data_end, covar_  );
    build_C(  C,1,size_d+1,data_begin, data_end,unk_begin, unk_end, covar_  );
    build_C(  C,size_d+1,size_d+1,unk_begin, unk_end,unk_begin, unk_end, covar_  );
    
    TNT::Cholesky_upper_factorization(C,L);

    if( size_d == 0 ) {
 //     TNT::Cholesky_upper_factorization(C,L);
      TNT::Vector< float > w(size_u);
      for(int i=1; i <= size_u; i++ ) 
        w(i) = marginal.inverse( gen_() );
      z = L*w;
    } 
    else  {
      //Build a vector with the data
      z.resize(size_d);
      int ind=1;
      for(GvalIterator it = data_begin; it != data_end; ++it, ++ind ) 
        z(ind) = it->property_value();
      

      TNT::Matrix< float > Ldd(size_d,size_d);
      for(int i=1;i<=size_d;i++)
        for(int j=1;j<=i;j++)
          Ldd(i,j)=L(i,j);
      TNT::Vector< float > wd  = Lower_triangular_solve(Ldd, z);

      TNT::Vector< float > w(size_u+size_d);
      for(int i=1;i<=size_d;i++) w(i) = wd(i);
      for(int i=size_d+1;i<=size_d+size_u;i++) w(i) = marginal.inverse( gen_() );

      z = L*w;
    }

    int ind=size_d+1;
    for( ; unk_begin != unk_end; ++unk_begin, ind++ ) {
      unk_begin->set_property_array(grid_->selected_property());
      unk_begin->set_property_value(z(ind));
    }


    return 0;
  }



protected:
  Covariance covar_;
  Rand_generator gen_;
  Grid* grid_;

  TNT::Matrix< float > L_;
  TNT::Vector< float > w_;
  int size_d_, size_u_;


};

#endif
/*
template<
  typename Geovalue,
  typename GvalIterator,
  typename Covariance,
  typename Rand_generator
>
 template<
    typename Geovalue,
    typename GvalIterator,
    typename MarginalCdf
  >
  int LU_simulator<Geovalue,GvalIterator,Covariance,Rand_generator>::operator()
      (GvalIterator unk_begin, GvalIterator unk_end,
                   GvalIterator data_begin , GvalIterator data_end, 
                   MarginalCdf marginal)
{
  //Compute the Lu simulation with
  //  z_u = C_ud * inv(C_dd)*z_d + L_uu * w
  // where d is data and u is unknown
  // L_uu is the L of the Cholesky decomposition of C_uu - C_ud * inv(C_dd) * C_ud'

  int size_uu = std::distance(unk_begin,unk_end);
  int size_dd = std::distance(data_begin,data_end);
  TNT::Matrix< float > Cuu(size_uu,size_uu);
  TNT::Matrix< float > Luu(size_uu,size_uu);
  TNT::Vector< float > simul(size_uu);
  //Build Cuu
  int ok = build_Cii(  Cuu, unk_begin, unk_end, covar_  );

    TNT::Vector< float > w(size_d);
    ind=1;
    for(GvalIterator it = data_begin; it != data_end; ++it, ++ind ) 
      w(ind) = marginal->inverse( gen_() );

  if( size_dd == 0 ) 
    TNT_lib::Cholesky_upper_factorization(Cuu,Luu);
    simul = luu*w;
  else {
    //Build a vector with the data
    TNT::Vector< float > z(size_d);
    ind=1;
    for(GvalIterator it = data_begin; it != data_end; ++it, ++ind ) 
      z(ind) = it->property_value();

    // Build Cdd
    ok = build_Cii(  Cdd, data_begin, data_end, covar_  );
    if(!ok) return 1;

    // Build Cud
    ok = build_C(  Cud, unk_begin, unk_end,data_begin, data_end, covar_  );
    if(!ok) return 1;

    //Compute Ctemp = C_ud * inv(C_dd)  transpose(const Matrix<T> &A)
    TNT::Matrix< float > M(size_uu,size_dd);
    ok = gauss_solve(Cuu,C_dd,M);
    if(!ok) return 1;

    ok = TNT::Cholesky_upper_factorization( Cuu-M*TNT::transpose(Cud),Luu );
    if(!ok) return 1;

    simul = M*z + Luu*w;
  }
  int ind = 1;
  for( ; unk_begin != unk_end; ++unk_begin, ind++ )
    unk_begin->set_value(simul(ind);

  return 0;
}
*/



/* Another way to compute the LU simulation, maybe faster ... maybe not
  Not working as it is.  must find a library to solve a linear sytem with
  matrix for RHS
    TNT::Matrix< float > Cuu(size_u,size_u);
    TNT::Matrix< float > Luu(size_u,size_u);
    TNT::Vector< float > simul(size_u);
    TNT::Vector< float > w(size_u);

    //Build Cuu
    int ok = build_Cii(  Cuu, unk_begin, unk_end, covar_  );
    int ind=1;
    for(int i=1; i <= size_u; i++ ) 
      w(i) = marginal.inverse( gen_() );

    if( size_d == 0 ) {
      TNT::Cholesky_upper_factorization(Cuu,Luu);
      simul = Luu*w;
    }  
    else {
      //Build a vector with the data
      TNT::Vector< float > z(size_d);
      ind=1;
      for(GvalIterator it = data_begin; it != data_end; ++it, ++ind ) 
        z(ind) = it->property_value();

      // Build Cdd
      TNT::Matrix< float > Cdd(size_d,size_d);
      ok = build_Cii(  Cdd, data_begin, data_end, covar_  );
      if(ok) return 1;

      // Build Cud
      TNT::Matrix< float > Cud(size_u,size_d);
      build_C(  Cud,1,1,unk_begin, unk_end,data_begin, data_end, covar_  );

      //Compute C_ud * inv(C_dd)  
      TNT::Matrix< float > M(size_u,size_d);
      ok = gauss_solve(Cuu,Cdd,M);
      if(ok) return 1;

      ok = TNT::Cholesky_upper_factorization( Cuu-M*TNT::transpose(Cud),Luu );
      if(ok) return 1;

      simul = M*z + Luu*w;
    }
    
    ind = 1;
    for( ; unk_begin != unk_end; ++unk_begin, ind++ ) {
 //     Geostat_grid* tmp_grid = dynamic_cast<Geostat_grid*>(unk_begin->grid());
      unk_begin->set_property_array(grid_->selected_property());
      unk_begin->set_property_value(simul(ind));
    }
*/
