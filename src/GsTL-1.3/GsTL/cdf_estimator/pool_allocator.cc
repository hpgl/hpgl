template<class T>
pool_allocator<T>::pool_allocator(int training_image_size, 
				  int template_size, int nb_of_categories) : 
  steps_(10),
  block_size_(compute_block_size(training_image_size, template_size, nb_of_categories,steps_)),
  treenode_ptr_pool_(block_size_*nb_of_categories, steps_),
  treenode_pool_(block_size_, steps_),
  pdf_pool_(block_size_*nb_of_categories, steps_) {
  
  allocated_ = 0;
}
  


template<class T>
int pool_allocator<T>::compute_upper_bound(int training_image_size, 
					   int template_size, int nb_of_categories){
  double upper_bound=0;
  double nb_of_cat = double(nb_of_categories);
  for(int i=0; i<=template_size; i++)
    upper_bound += std::min( pow(nb_of_cat,i) , double(training_image_size));


  return int(upper_bound);
}



template<class T>
int pool_allocator<T>::compute_block_size(int training_image_size,
					  int template_size, int nb_of_categories,
					  int steps){
  int upper = compute_upper_bound(training_image_size,template_size,nb_of_categories);
  upper /= steps;
  if(upper > 1)
    return upper;
  else 
    return 1;
}




template<class T>
int pool_allocator<T>::nb_of_allocated_nodes(){
  return treenode_pool_.size();
}



#ifdef __GNUC__
#pragma implementation
#endif
#include "pool_allocator.h"
