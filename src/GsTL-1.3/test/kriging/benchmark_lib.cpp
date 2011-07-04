#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>


#include <GsTL/kriging/kriging_weights.h>
#include <GsTL/kriging/SK_constraints.h>
#include <GsTL/kriging/OK_constraints.h>
#include <GsTL/kriging/KT_constraints.h>

#include <GsTL/matrix_library/tnt/stopwatch.h> 

#include <stdlib.h>

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
  inline Point& location() { return loc_;};
  inline const Point& location() const { return loc_;};
  
  inline double& property_value(){return pval_;}
  inline const double& property_value() const {return pval_;}

private:
  Point loc_;
  double pval_;
};

std::ostream& operator << ( std::ostream& os, Node& N ){
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
    return exp(-3*h/a);
  } 
};


struct Covariance_set {
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
  Node& operator[](int i) {return neigh_[i];}
  
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

  Point center(41,29);

  // The numbering of the nodes corresponds to gslib's output order
  Node P5(38,28,0.5740);
  Node P8(45,29,1.2110);
  Node P4(41,26,2.1270);
  Node P3(39,31,8.3400);
  Node P6(38,31,18.6420);
  Node P2(39,30,7.9380);
  Node P7(39,32,2.2840);
  Node P1(40,31,2.5090);
  Node P9(37,20,0.5740);
  Node P10(25,28,1.2110);
  Node P11(39,26,2.1270);
  Node P12(32,31,8.3400);
  Node P13(30,34,18.6420);
  Node P14(33,35,7.9380);
  Node P15(42,32,2.2840);
  Node P16(31,23,2.5090);

  neighborhood voisin;
  voisin.add_node(P1);
  voisin.add_node(P2);
  voisin.add_node(P3);
  voisin.add_node(P4);
  voisin.add_node(P5);
  voisin.add_node(P6);
  voisin.add_node(P7);
  voisin.add_node(P8);
  voisin.add_node(P9);
  voisin.add_node(P10);
  voisin.add_node(P11);
  voisin.add_node(P12);
  voisin.add_node(P13);
  voisin.add_node(P14);
  voisin.add_node(P15);
  voisin.add_node(P16);
  


  

  typedef matrix_lib_traits<TNT_lib<double> >::Vector TNTvector;

  covariance covar;
  
  //______________________________
  // Simple Kriging
  //______________________________

  std::cout << std::endl <<std::endl;
  std::cout << "____________________________________________" 
	    << std::endl
	    << "Simple kriging"
	    << std::endl << std::endl;

  TNTvector SK_weights;
  SK_constraints SK;
  OK_constraints OK;
  double sk_variance;
  

  TNT::stopwatch chrono;
  chrono.start();
  for(int count = 0 ; count < 50000 ; count ++) {
    int change=1;
    if(drand48() <0.5) change = -1;
    (voisin[0].location())[0] += change;
    kriging_weights<GSTL_TNT_lib>(SK_weights,
				  center, voisin,
				  covar, OK );
  }

  chrono.stop();
  std::cout << "time elapsed using Gauss: " << chrono.read() << std::endl;
  
  chrono.reset();
  chrono.start();
  for(int count = 0 ; count < 50000 ; count ++) {
    int change=1;
    if(drand48() <0.5) change = -1;
    voisin[0].property_value() = 0.5*count;
    kriging_weights(SK_weights, 
		    center, voisin,
		    covar, OK );
  }

  chrono.stop();
  std::cout << "time elapsed using LU: " << chrono.read() << std::endl;
  
  
  /*
  //______________________________
  // Ordinary Kriging
  //______________________________

  std::cout << std::endl <<std::endl;
  std::cout << "____________________________________________" 
	    << std::endl
	    << "Ordinary kriging"
	    << std::endl << std::endl;

  TNTvector OK_weights;
  OK_constraints OK;
  double ok_variance;
  status = kriging_weights(OK_weights, ok_variance,
			   center, voisin,
			   covar, OK);
						   
  std::cout << "Here are the weights:" << std::endl
	    << OK_weights << std::endl;

  
  

  //______________________________
  // Kriging with Trend
  //______________________________

  std::cout << std::endl <<std::endl;
  std::cout << "____________________________________________" 
	    << std::endl
	    << "Kriging with Trend"
	    << std::endl << std::endl;
  
  TNTvector KT_weights;
  KT_constraints<functIter> KT(functArray.begin(),functArray.end());
  double kt_variance;
  status = kriging_weights(KT_weights, kt_variance,
			   center, voisin,
			   covar, KT);

						  
  std::cout << "Here are the weights:" << std::endl
	    << KT_weights << std::endl;

  */

  return 0;


}
