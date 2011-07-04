template<class T>
standard_allocator<T>::standard_allocator(int training_image_size,
					  int template_size, int nb_of_categories){
  nb_of_allocated_nodes_=0;
}



template<class T>
void standard_allocator<T>::delete_node(treenode_iterator it, 
					int nb_of_categories){

  for(int i=0; i< nb_of_categories; i++){
    if(it->child(i)!=0)
      delete_node(it->child(i),nb_of_categories);
  }

  delete [] it->get_child_ptr();
  delete [] it->get_pdf_ptr();
  delete it;
  nb_of_allocated_nodes_ --;
}

#ifdef __GNUC__
#pragma implementation
#endif
#include "standard_allocator.h"
