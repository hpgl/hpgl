#ifndef __GTL_TREE_NODE_STANDARD_ALLOCATOR_ 
#define __GTL_TREE_NODE_STANDARD_ALLOCATOR_
#ifdef __GNUC__
#pragma interface
#endif


template<class T, class allocator>
class tree_node;


/** The standard allocator allocates memory only when 
 * it is needed: it does not create a pool of memory for example.
 */ 

template<class T>
class standard_allocator{
  
 public:

  typedef tree_node<T,standard_allocator> treenode;
  typedef T* category_type_iterator;
  typedef treenode*  treenode_iterator;
  typedef treenode**  treenode_ptr_iterator;
  
  standard_allocator(int training_image_size, int template_size, int nb_of_categories);
  
  category_type_iterator new_pdf(int nb_of_categories);
  treenode_ptr_iterator new_node_ptr(int nb_of_categories);
  treenode_iterator new_node(int nb_of_categories);
  
  void delete_node(treenode_iterator it, int nb_of_categories);

  inline int nb_of_allocated_nodes();


  
 private:
  
  int nb_of_allocated_nodes_;

}; // end of class standard_allocator



template<class T>
inline int standard_allocator<T>::nb_of_allocated_nodes(){
  return nb_of_allocated_nodes_;
}


template<class T>
inline typename standard_allocator<T>::category_type_iterator
 standard_allocator<T>::new_pdf(int nb_of_categories){

  category_type_iterator new_pdf = new T[nb_of_categories];
  for(int i=0; i< nb_of_categories; i++)
    new_pdf[i]=0;

  return new_pdf;
}


template<class T>
inline typename standard_allocator<T>::treenode_ptr_iterator
 standard_allocator<T>::new_node_ptr(int nb_of_categories){

  treenode_ptr_iterator new_node_it = new treenode*[nb_of_categories];
  for(int i=0; i< nb_of_categories; i++)
    new_node_it[i]=0;

  return new_node_it;
}

template<class T>
inline typename standard_allocator<T>::treenode_iterator
 standard_allocator<T>::new_node(int nb_of_categories){

  treenode_iterator new_node = new treenode(this,nb_of_categories);
  nb_of_allocated_nodes_ ++;
  return new_node;
}



#if defined(__GNUC__) || defined(WIN32)
#include "standard_allocator.cc"
#elif defined(__xlC__)
#pragma implementation("standard_allocator.cc")
#endif

#endif
