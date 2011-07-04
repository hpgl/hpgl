#include <GsTL/kriging/kriging_weights.h>
#include <GsTL/kriging/SK_constraints.h>
#include <GsTL/kriging/OK_constraints.h>
#include <GsTL/kriging/KT_constraints.h>
#include <GsTL/kriging/linear_combination.h>
#include <GsTL/kriging/cokriging_weights.h>

#include <GsTL/cdf_estimator/gaussian_cdf_Kestimator.h>
#include <GsTL/cdf/gaussian_cdf.h>

#include <GsTL/geometry/covariance.h>

#include <GsTL/kriging/kriging_constraints.h>
#include <GsTL/kriging/kriging_combiner.h>

#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>



class Point{
public:
  typedef double coordinate_type;

  Point(){X=0;Y=0;};
  Point(int x, int y): X(x), Y(y) {};
  int& operator[](int i){
    if(i==1) return X;
    else     return Y;
  }

  const int& operator[](int i) const {
    if(i==1) return X;
    else     return Y;
  }
	       
  int X;
  int Y;
};


//-------------------------


class Node{
public:
  typedef double property_type;
  typedef Point location_type;

  Node():pval_(0) {loc_.X=0; loc_.Y=0;};
  Node(int x, int y, double pval){loc_.X=x; loc_.Y=y;pval_=pval;};
  inline Point location() const { return loc_;};
  inline double& property_value(){return pval_;}
  inline const double& property_value() const {return pval_;}

private:
  Point loc_;
  double pval_;
};

std::ostream& operator << (std::ostream& os, Node& N){
  std::cout << N.location().X << " " 
	    << N.location().Y << " ";
  
  return os;
};


//---------------------------


class covariance{
public:
  inline double operator()(const Point& P1,const Point& P2) const {
    double h=sqrt(pow(P1.X-P2.X,2)+pow(P1.Y-P2.Y,2));
    double a=10;
    double c=1.0;
    return c * exp(-3*h/a);
  } 
};


struct My_covariance_set {
  inline double operator()(int i, int j, 
			   const Point& P1,const Point& P2) const {
    double h=sqrt(pow(P1.X-P2.X,2)+pow(P1.Y-P2.Y,2));
    double a=10;
    return exp(-3*h/a);
  }
};

//-----------------------------


class neighborhood{
public:
  typedef std::vector<Node>::iterator iterator;
  typedef std::vector<Node>::const_iterator const_iterator;

  void add_node(Node n){neigh_.push_back(n);};
  iterator begin(){return neigh_.begin();};
  iterator end(){return neigh_.end();};
  const_iterator begin() const {return neigh_.begin();};
  const_iterator end() const {return neigh_.end();};
  unsigned int size() const {return neigh_.size();}
  bool is_empty() const {return neigh_.empty();}

private:
  void find_neighbors(Point& u);

  std::vector<Node> neigh_;
};

//-------------------------------


inline double id(Point loc){
  return 1.0 ;}

inline double fx(Point loc){
  return loc.X;}

inline double fy(Point loc){
  return loc.Y;}

inline double fxy(Point loc){
  return loc.X * loc.Y;}

class functor{
public:
  functor() : fp(0) {}
  functor(double (*f)(Point)) : fp(f) {}

  inline double operator()(Point loc) const {
    return (*fp)(loc);}

private:
  double (*fp) (Point);
};




//____________________________________________________________

int main() {

  Node center_node(41,29,-99);

  // The numbering of the nodes corresponds to gslib's output order
  Node P5(38,28,0.5740);
  Node P8(45,29,1.2110);
  Node P4(41,26,2.1270);
  Node P3(39,31,8.3400);
  Node P6(38,31,18.6420);
  Node P2(39,30,7.9380);
  Node P7(39,32,2.2840);
  Node P1(40,31,2.5090);


  neighborhood voisin;
  voisin.add_node(P1);
  voisin.add_node(P2);
  voisin.add_node(P3);
  voisin.add_node(P4);
  voisin.add_node(P5);
  voisin.add_node(P6);
  voisin.add_node(P7);
  voisin.add_node(P8);

 

  
  typedef TNT_lib<double> TNT;
  typedef matrix_lib_traits<TNT>::Vector TNTvector;
  typedef matrix_lib_traits<TNT>::Symmetric_matrix TNTMatrix;

  covariance covar;


  
  //_____________________________
  // gaussian cdf estimator 
  //_____________________________

  std::cout << std::endl <<std::endl;
  std::cout << "____________________________________________" 
	    << std::endl
	    << "estimating gaussian cdf with SK"
	    << std::endl << std::endl;

  Gaussian_cdf g_ccdf;

  typedef Kriging_combiner<std::vector<double>::const_iterator,
                           neighborhood>   KCombiner;
  KCombiner sk_combiner( new SK_combiner<std::vector<double>::const_iterator,
                                          neighborhood>( 9.0 )   );
  typedef Kriging_constraints<neighborhood, Point, TNT> KConstraints;
  KConstraints constraints( new SKConstraints_impl<neighborhood, Point,TNT> );


  Gaussian_cdf_Kestimator<covariance,
    neighborhood,KConstraints,TNT>  gK_estimator( covar, constraints, sk_combiner ); 
  
  gK_estimator(center_node, voisin, g_ccdf);
  
  std::cout << "gaussian cdf : mean= " << g_ccdf.mean() << "   variance= " 
	    << g_ccdf.variance() << std::endl;


  /*  
  Gaussian_cdf g_ccdf2;
  
  Gaussian_cdf_Kestimator<covariance,
    neighborhood, SK_constraints>  gK_estimator2( covar, SK_constraints(), sk_combiner2 ); 
  gK_estimator2(center, voisin, g_ccdf2);
  
  std::cout << "gaussian cdf : mean= " << g_ccdf2.mean() << "   variance= " 
       << g_ccdf2.variance() << std::endl;
  */
}
