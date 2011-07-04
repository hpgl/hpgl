//-----------------------------
// Constructor

template<class T, class allocator>
tree_node<T,allocator>::tree_node(){
  pdf_ = 0;
  child_node_ = 0;

}


template<class T, class allocator>
tree_node<T,allocator>::tree_node(allocator* memory_manager_, int nb_of_categories){
  pdf_ = memory_manager_-> new_pdf(nb_of_categories);
  child_node_ = memory_manager_-> new_node_ptr(nb_of_categories);

}






#ifdef __GNUC__
#pragma implementation
#endif
#include "tree_node.h"
