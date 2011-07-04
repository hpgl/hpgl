#ifndef __GTL_SEARCH_TREE__ 
#define __GTL_SEARCH_TREE__
#ifdef __GNUC__
#pragma interface
#endif



#include "tree_node.h"
#include "pool_allocator.h"
#include <GsTL/utils/gstl_error_messages.h>

#include <vector>
#include <algorithm>
#include <iterator>

#include <iostream>


/** The structure implemented corresponds to the search tree structure
 * described in Strebelle's PhD thesis (2000).
 * The class is templated by two arguments:
 *  - T is the type of the property the search tree is built for (it is usually
 *    an integer type since search trees are built for categorical variables).
 *  - allocator is a way to allocate memory. By default, memory is allocated by
 *  chuncks. 
 *
 */

template<class T=int, class allocator=pool_allocator<int> >
class search_tree{

 public:

  typedef T category_type;
  
  /** constructor
   * Neighborhood neighbors does not need to contain any neighbor when  
   * this function is called.
   */
  template<class neighborhood, class forward_iterator>
  search_tree(forward_iterator first, forward_iterator last,
	      neighborhood& neighbors, int max_neigh_size,
	      unsigned int nb_of_categories,
	      int min_replicates = 10 );

  // destructor
  ~search_tree();

  int nb_of_categories() {return nb_of_categories_;}





  template<class Geovalue_, class neighborhood, class non_param_cdf>
  int operator()(const Geovalue_& u, 
		 neighborhood& neighbors,
		 non_param_cdf& ccdf);
  
  
 /** This function is there mainly for debugging purposes.
  * The search tree is displayed branch by branch. The display goes down a
  * branch as much as possible. When it reaches a terminal leaf, it goes up
  * one level and tries to go down again, as deep as possible. And so on.
  * This function is useful only for small trees. 
  * For each node, it shows:
  *  - which way we had to go to reach that node: for a binary tree, (0) means
  *    it is the left child of the previous node, (1) is the right child.
  *  - the level of the node (level 0 is the root of the tree)
  *  - the pdf at that node
  *  - "end" means the node has no child  
  * 
  */
  void display(std::ostream& os);

  inline long int size() {return memory_manager_.nb_of_allocated_nodes();}



 private:
  
  typedef typename allocator::treenode_iterator treenode_iterator;
  
  treenode_iterator root_;
  int nb_of_categories_; 
  int min_replicates_;
  
  allocator memory_manager_;


 private:

  /** Update the search tree: either update the element value
   * or add a new branch. The update is based on the search tree
   * construction described in Strebelle's SCRF paper 1999 and 2000.
   * This function assumes that the property value of the geovalues is
   * discrete, between 0 and nb_of_categories.
   */
  template<class neighborhood>
    void update(treenode_iterator t_node,
		T category_of_center,
		neighborhood& neighbors,
		typename neighborhood::iterator current_it);


  /** Retrieves the pdf given a data event. 
   * This function is recursive. 
   * If the data event does not exist, it is simplified and looked for, anew.  
   *
   * @param result is a vector of pointers to the tree_nodes found.
   * If all the neighbors are estimated, then result will contain only one
   * pointer. If one of the neighbors was not estimated, result contains multiple
   * pointers. It is then necessary to select the correct one(s). This will be perfomed
   * by the search tree.
   * @param neighbors is a model of concept Neighborhood.
   * @param neigh_it is an iterator to the elements of neighbors. It is used by the 
   * recursion to determine whether or not all the neighbors have been taken into 
   * account.
   */

  template<class neighborhood>
  void get_all_pdfs(std::vector<treenode_iterator>& result,
		    treenode_iterator t_node,
		    neighborhood& neighbors,
		    typename neighborhood::const_iterator neigh_it);


  void display_all_children(std::ostream& os, treenode_iterator TN);

  /** The copy constructor and assignment operator are declared
   * here to avoid any accidental use of their default counterparts.
   */
  search_tree(const search_tree<T,allocator>& rhs);
  search_tree& operator=(const search_tree<T,allocator>& rhs);

}; // end of class search_tree


#if defined(__GNUC__) || defined(WIN32)
#include "search_tree.cc"
#elif defined(__xlC__)
#pragma implementation("search_tree.cc")
#endif

#endif
