#ifndef __GTL_TREE_NODE__ 
#define __GTL_TREE_NODE__
#ifdef __GNUC__
#pragma interface
#endif



#include <GsTL/utils/gstl_error_messages.h>

#include <vector>
#include <iostream>


/** This class implements a node of the "search tree" structure
 * described in Strebelle's PhD thesis (2000).
 */

template<class T, class allocator>
class tree_node{
  
 public:
  
  typedef T value_type;
  typedef typename allocator::category_type_iterator value_type_iterator;
  typedef typename allocator::treenode_ptr_iterator treenode_ptr_iterator;
  typedef typename allocator::treenode_iterator treenode_iterator;
 

  tree_node();
  tree_node(allocator* memory_manager_, int nb_of_categories);

    

  // Access the i-th element (the first is element 0) of pdf_.
  inline value_type& pdf(int i);
  inline const value_type& pdf(int i) const;
  
  // Access to the i-th child (first is i=0) of a tree node.
  inline treenode_iterator& child(int i);
  inline const treenode_iterator& child(int i) const;

  inline int& level() {return level_;}
  inline const int& level() const {return level_;}

  inline value_type_iterator get_pdf_ptr() {return pdf_;}
  inline treenode_ptr_iterator get_child_ptr() {return  child_node_;}

  inline bool has_child( int i, int min_replicates, int classes );

 private:
  
  value_type_iterator    pdf_;
  treenode_ptr_iterator  child_node_;
  
  int        level_;    

}; // end of class tree_node




//------------------------------
// inline Access functions

template<class T, class allocator>
inline typename tree_node<T,allocator>::value_type&
tree_node<T,allocator>::pdf(int i) {
  //  gstl_assert(i>=0 && i<nb_of_categories);
  return pdf_[i];
} 

template<class T, class allocator>
inline const typename tree_node<T,allocator>::value_type&
tree_node<T,allocator>::pdf(int i) const {
//   gstl_assert(i>=0 && i<nb_of_categories);
  return pdf_[i];
} 
  
template<class T, class allocator>
inline const typename tree_node<T,allocator>::treenode_iterator&
 tree_node<T,allocator>::child(int i) const {
 //    gstl_assert(i>=0 && i<nb_of_categories);
    return child_node_[i];
  }
  
template<class T, class allocator>
inline typename tree_node<T,allocator>::treenode_iterator&
 tree_node<T,allocator>::child(int i) {
//     gstl_assert(i>=0 && i<nb_of_categories);
    return child_node_[i];
}



template<class T, class allocator>
inline bool
tree_node<T,allocator>::has_child( int id, int min_replicates, int classes ) {
  if( child_node_[id] == 0 ) return false;
  
  // There is a child. Check if it has enough replicates
  long int replicates = 0;
  for( int i=0; i< classes; i++ ) {
    replicates += child_node_[id]->pdf_[i];
  }

  return (replicates >= min_replicates );
}



//------------------------------
// Two functors to compare tree_nodes

template<class T, class allocator>
struct tree_node_lesser_than{
  bool operator()(tree_node<T,allocator>* T1, tree_node<T,allocator>* T2){
    return T1->level() < T2->level();
  } 
};

template<class T, class allocator>
struct tree_node_greater_than{
  bool operator()(tree_node<T,allocator>* T1, tree_node<T,allocator>* T2){
    return T1->level() > T2->level();
  } 
};




#if defined(__GNUC__) || defined(WIN32)
#include "tree_node.cc"
#elif defined(__xlC__)
#pragma implementation("tree_node.cc")
#endif

#endif
