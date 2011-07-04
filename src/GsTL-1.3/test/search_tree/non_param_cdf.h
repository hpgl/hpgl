#ifndef __GTL_NON_PARAM_CDF_EXAMPLE__
#define __GTL_NON_PARAM_CDF_EXAMPLE__

class non_param_cdf{

 private:

  int categ_[2];
  double prob_[2];


 public:

  typedef int value_type;
  typedef double* p_iterator;
  typedef int* z_iterator;
  
  non_param_cdf(int nb_of_categories=2) {
    for(int i=0; i<nb_of_categories ; i++)
      categ_[i]=i;
  };

  inline z_iterator z_begin(){return categ_;}
  inline z_iterator z_end(){return categ_+2;}
  inline p_iterator p_begin(){return prob_;}
  inline p_iterator p_end(){return prob_+2;}
  
};


#endif
