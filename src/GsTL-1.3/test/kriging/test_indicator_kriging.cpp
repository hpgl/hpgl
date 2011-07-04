#include <GsTL/kriging/SK_constraints.h>
#include <GsTL/kriging/OK_constraints.h>
#include <GsTL/kriging/KT_constraints.h>

#include <GsTL/cdf_estimator/indicator_cdf_estimator.h>
#include <GsTL/cdf/non_param_cdf.h>
#include <GsTL/cdf/categ_non_param_cdf.h>

#include <GsTL/geometry/covariance.h>

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




//____________________________________________________________

int main() {

  //Point center(41,29);
  Node center(41,29,-99);

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

  covariance covar;



  //_____________________________
  // indicator cdf estimator 
  //_____________________________

  std::cout << std::endl <<std::endl;
  std::cout << "____________________________________________" 
	    << std::endl
	    << "estimating indicator cdf with SK"
	    << std::endl << std::endl;

  double marginal[5]={0.2, 0.3, 0.1, 0.1, 0.3};
  Indicator_cdf_estimator<covariance*> indicator_estimator(&covar, &covar+1,
							   marginal, marginal+5);

  double z_values[5] = {0.1, 0.4, 0.9, 1.4, 2.1};

  Non_param_cdf<> ccdf(z_values, z_values+5);


  indicator_estimator(center, &voisin, &voisin+1, ccdf);

  for(int i=0; i<5; i++)
    std::cout << ccdf.prob(z_values[i]) << "  " ;
  std::cout << std::endl;

  return 0;
}
