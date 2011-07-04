#ifndef __GTL_TREE_NODE_POOL_ALLOCATOR_ 
#define __GTL_TREE_NODE_POOL_ALLOCATOR_
#ifdef __GNUC__
#pragma interface
#endif


#include "twoD_array.h"

#include <math.h>
#include <vector>
#include <algorithm>


template<class T, class allocator>
class tree_node;


template<class T>
class pool_allocator{
  
 public:

  typedef tree_node<T,pool_allocator> treenode;
  typedef typename twoD_array<T>::iterator  category_type_iterator;
  typedef typename twoD_array<treenode>::iterator  treenode_iterator;
  typedef typename twoD_array<treenode*>::iterator  treenode_ptr_iterator;
  
  pool_allocator(int training_image_size, int template_size, int nb_of_categories);
  
  //category_type_iterator new_pdf(int nb_of_categories);
  category_type_iterator new_pdf(int nb_of_categories){
    category_type_iterator new_element_position = pdf_pool_.push_back(0);

    for(int i=1; i<nb_of_categories ; i++)
      pdf_pool_.push_back(0);

    return new_element_position;
  }



  treenode_ptr_iterator new_node_ptr(int nb_of_categories);
  treenode_iterator new_node(int nb_of_categories);
  
  void delete_node(treenode_iterator it, int nb_of_categories);

  int nb_of_allocated_nodes();
  int nb_of_nodes_control() {return allocated_;};
  

 private:
  
  const int steps_;
  int block_size_;
  twoD_array<treenode*> treenode_ptr_pool_;
  twoD_array<treenode> treenode_pool_;
  twoD_array<T> pdf_pool_;
  
  int compute_upper_bound(int training_image_size,
			  int template_size, int nb_of_categories);
  int compute_block_size(int training_image_size,
			 int template_size, int nb_of_categories,
			 int steps);

  int allocated_;
}; // end of class pool_allocator




/*
template<class T>
inline typename pool_allocator<T>::category_type_iterator 
pool_allocator<T>::new_pdf(int nb_of_categories){

  category_type_iterator new_element_position = pdf_pool_.push_back(0);

  for(int i=1; i<nb_of_categories ; i++)
    pdf_pool_.push_back(0);

  return new_element_position;
}
*/


template<class T>
inline typename pool_allocator<T>::treenode_ptr_iterator 
pool_allocator<T>::new_node_ptr(int nb_of_categories){
  treenode_ptr_iterator new_element_position= treenode_ptr_pool_.push_back(0);

  for(int i=1; i<nb_of_categories ; i++)
    treenode_ptr_pool_.push_back(0);

  return new_element_position;
}


template<class T>
inline typename pool_allocator<T>::treenode_iterator 
pool_allocator<T>::new_node(int nb_of_categories){
  treenode TN(this,nb_of_categories);
  treenode_iterator new_element_position = treenode_pool_.push_back(TN);
 
  allocated_++;

  return new_element_position;
}



template<class T>
inline void 
pool_allocator<T>::delete_node( typename pool_allocator<T>::treenode_iterator , int ) {
  // Don't do anything
}


#if defined(__GNUC__) || defined(WIN32)
#include "pool_allocator.cc"
#elif defined(__xlC__)
#pragma implementation("pool_allocator.cc")
#endif

#endif
