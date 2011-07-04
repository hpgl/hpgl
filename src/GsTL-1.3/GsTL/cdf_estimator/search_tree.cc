//-----------------------------------
// Constructor

template<class T, class allocator>
template<class neighborhood, class forward_iterator>
search_tree<T, allocator>::search_tree(forward_iterator first, forward_iterator last,
				       neighborhood& neighbors, int max_neigh_size, 
				       unsigned int nb_of_categ,
				       int min_replicates)
  : nb_of_categories_(nb_of_categ),
    min_replicates_( min_replicates ),
    memory_manager_(std::distance(first,last), max_neigh_size, nb_of_categ){

  root_ = memory_manager_.new_node(nb_of_categ);
  root_->level()=0;
  
  /** build the search tree from the information in range [first,last)
   */

  for(forward_iterator it=first; it!=last; ++it)
  {
      if ( !( it->is_informed() ) )    continue;

      T current_category = static_cast<T>( it->property_value() );
      gstl_assert(current_category>=0 && current_category< nb_of_categories_);
      root_->pdf(current_category)++;

      // find the neighbors of current location
      neighbors.find_neighbors( *it );

      update(root_, current_category, neighbors, neighbors.begin() );
  }
}




//-----------------------------------
// Destructor

template<class T, class allocator>
search_tree<T,allocator>::~search_tree(){

  memory_manager_.delete_node(root_, nb_of_categories_);
}




//-----------------------------------
// update (private function)

template<class T, class allocator>
template<class neighborhood>
void search_tree<T,allocator>::update(treenode_iterator t_node,
				      T category_of_center,
				      neighborhood& neighbors,
				      typename neighborhood::iterator current_it){

  /* Go through all the neighbors, or until one of the neighbors is not 
   * informed (because the neighborhood is on the edge of the image).
   * For each neighbor (actually a "while" loop):
   *  - get its property value.
   *  - Go down to the corresponding tree node. If the node does not exist,
   *    create it first
   *  - Update the pdf of the node.
   *  end loop
   */
  while( (current_it != neighbors.end()) &&
	 current_it->is_informed() ) {

    // typedef typename neighborhood::value_type::property_type gval_property_type;
    int prop_value = static_cast<int>( current_it->property_value() );
    
    gstl_assert(prop_value >=0 && prop_value < nb_of_categories_);
    
    // if the child does not exist, create it
    if(t_node->child(prop_value)==0){
      t_node->child(prop_value) = memory_manager_.new_node(nb_of_categories_);
      t_node->child(prop_value)->level() = t_node->level()+1;
    }

    // update the tree with the new category:
    t_node->child(prop_value)->pdf(category_of_center) ++;

    // go to the next neighbor 
    ++current_it;

    // go one level down in the tree 
    t_node = t_node->child(prop_value);
  }
}





//-------------------------------
// get all pdfs (private function)

/* This function is recursive. The aim of each call is to go down one level
 * in the tree. The stopping condition is if we are at a terminal leaf or 
 * there are no conditioning data (informed neighbor) left.
 * How does it work?
 * This function stores all the terminal nodes it has found into vector "result".
 * There can be multiple terminal node found because if one of the neighbors is
 * not informed, the search is done for all the values this node could have 
 * taken. If all the neighbors are informed, "result" will only contain one node.
 */
/*
template<class T, class allocator>
template<class neighborhood>
void search_tree<T,allocator>::get_all_pdfs(std::vector<treenode_iterator>& result,
					    treenode_iterator t_node,
					    neighborhood& neighbors,
					    typename neighborhood::const_iterator neigh_it){
  // if the node is informed
  if( neigh_it->is_informed() ) {
    category_type current_category = category_type( neigh_it->property_value() );
    
    //if the branch of the tree actually exists:
    if( t_node->has_child( current_category, min_replicates_, nb_of_categories_ ) ) {
      
      // If there are no neighbors left, the search is over
      if ( ++neigh_it == neighbors.end() ){
	treenode_iterator node_ptr = t_node->child(current_category);
	result.push_back(node_ptr);
      }
      else
	// recursive call
	get_all_pdfs(result, t_node->child(current_category),
		     neighbors, neigh_it);
    }
    // the branch does not exist: the search is over
    else
      result.push_back(t_node);
  }

  // the node is not informed.
  // continue the search by assigning to the node any possible value
  else {
    for (int i=0; i<nb_of_categories_ ; i++){
      typename neighborhood::const_iterator next_neigh = neigh_it;
	  
      //if the branch of the tree actually exists:
      if(t_node->has_child( i, min_replicates_, nb_of_categories_ ) ) {
	// If there are no neighbors left, the search is over
	if( ++next_neigh == neighbors.end() )
	  result.push_back(t_node->child(i));
	else
	  // recursive call
	  get_all_pdfs(result, t_node->child(i), 
		       neighbors, next_neigh);
      }
      // the branch does not exist: the search is over
      else
	result.push_back(t_node);
    }
  }
}
//*/

/*
template<class T, class allocator>
template<class neighborhood>
void search_tree<T,allocator>::get_all_pdfs(std::vector<treenode_iterator>& result,
					    treenode_iterator t_node,
					    neighborhood& neighbors,
					    typename neighborhood::const_iterator neigh_it){

  // if the node is informed
  if( neigh_it->is_informed() ) {
    category_type current_category = category_type( neigh_it->property_value() );
    
    //if the branch of the tree actually exists:
    if( t_node->child(current_category) != 0 ) {
      
      // If there are no neighbors left, the search is over
      if ( ++neigh_it == neighbors.end() ){
	treenode_iterator node_ptr = t_node->child(current_category);
	result.push_back(node_ptr);
      }
      else
	// recursive call
	get_all_pdfs(result, t_node->child(current_category),
		     neighbors, neigh_it);
    }
    // the branch does not exist: the search is over
    else
      result.push_back(t_node);
  }

  // the node is not informed.
  // continue the search by assigning to the node any possible value
  else {
    for (int i=0; i<nb_of_categories_ ; i++){
      typename neighborhood::const_iterator next_neigh = neigh_it;
	  
      //if the branch of the tree actually exists:
      if(t_node->child(i) != 0) {
	// If there are no neighbors left, the search is over
	if( ++next_neigh == neighbors.end() )
	  result.push_back(t_node->child(i));
	else
	  // recursive calll
	  get_all_pdfs(result, t_node->child(i), 
		       neighbors, next_neigh);
      }
      // the branch does not exist: the search is over
      else
	result.push_back(t_node);
    }
  }
}
*/

// modified by JBWu, to account for min_replicates_ input
template<class T, class allocator>
template<class neighborhood>
void search_tree<T,allocator>::get_all_pdfs(std::vector<treenode_iterator>& result,
					    treenode_iterator t_node,
					    neighborhood& neighbors,
					    typename neighborhood::const_iterator neigh_it)
{
    result.push_back(t_node);     // record it anyway, in case of dropping nodes

    if( neigh_it->is_informed() )     
    {    // if the node is informed
        category_type current_category = category_type( neigh_it->property_value() );

        if( t_node->child(current_category) != 0 )      
        {   //if the branch of the tree actually exists:
            if ( ++neigh_it == neighbors.end() )
            {    // If there are no neighbors left, the search is over
                treenode_iterator node_ptr = t_node->child(current_category);
                result.push_back(node_ptr);
            }
            else   // recursive call        
                get_all_pdfs(result, t_node->child(current_category), neighbors, neigh_it);
        }        
    }
    else     // the node is not informed.
    {        // continue the search by assigning to the node any possible value
        for (int i=0; i<nb_of_categories_ ; i++)
        {
            typename neighborhood::const_iterator next_neigh = neigh_it;

            if(t_node->child(i) != 0) 
            {        //if the branch of the tree actually exists:
                if( ++next_neigh == neighbors.end() )       // If there are no neighbors left, the search is over
                    result.push_back(t_node->child(i));
                else      // recursive call
                    get_all_pdfs(result, t_node->child(i), neighbors, next_neigh);
            }
        }
    }
}

//------------------------------
// operator ()

template<class T, class allocator>
template<class Geovalue_, class neighborhood, class non_param_cdf>
int search_tree<T,allocator>::operator()(const Geovalue_& u, 
					 neighborhood& neighbors,
					 non_param_cdf& ccdf){

  typedef std::vector<treenode_iterator> node_ptr_vector;
  typedef typename node_ptr_vector::iterator node_ptr_vector_iterator;
  typedef typename non_param_cdf::p_iterator p_iterator;

  // initialize the ccdf
  p_iterator it = ccdf.p_begin();
  for(int l=0 ; l < nb_of_categories_ ; ++l, ++it)
    *it = 0;

  node_ptr_vector retrieved_pdfs;
  int max_level = -1;


  // If there are no conditioning data (ie neighbors.size()==0), the cpdf
  // is the pdf given by the root of the tree, ie the marginal pdf of the 
  // training image.

  if( neighbors.size() == 0) {
    retrieved_pdfs.push_back(root_);
    max_level = root_->level();
  }
  else {
    get_all_pdfs(retrieved_pdfs,
		 root_, neighbors, neighbors.begin() );
    
    // At this point retrieved_pdfs contains pointers to tree_nodes of possibly
    // different levels. We are only interested in the tree_node(s) with the highest level
    // (i.e. deepest node(s), those that used the most conditioning data).
    
    typedef tree_node_lesser_than<T,allocator> TN_lesser_than;
    node_ptr_vector_iterator max_elem_ptr = std::max_element(retrieved_pdfs.begin(),
							     retrieved_pdfs.end(),
							     TN_lesser_than() );
    max_level = (*max_elem_ptr)->level();
  }

  // put the pdf into the ccdf 
  double replicates_nb = 0;

  /*
  node_ptr_vector_iterator  vec_it = retrieved_pdfs.begin();
  for( ; vec_it!=retrieved_pdfs.end(); ++vec_it) {
    if( (*vec_it)->level() == max_level) {
      it = ccdf.p_begin();
      for(int i=0 ; i < nb_of_categories_; ++i, ++it) {
	replicates_nb += (*vec_it)->pdf(i);
	*it += (*vec_it)->pdf(i);
      }
    }
  }
  */

  // rewrite to account for different min_replicates input 
  int current_level = max_level+1;

  while ( replicates_nb<min_replicates_ && current_level>0 )
  {
      current_level --;
      replicates_nb = 0;

      for(node_ptr_vector_iterator  vec_it = retrieved_pdfs.begin(); vec_it!=retrieved_pdfs.end(); ++vec_it) 
      {
          if( (*vec_it)->level() == current_level) 
          {
              it = ccdf.p_begin();
              for(int i=0 ; i < nb_of_categories_; ++i, ++it) 
              {
                  replicates_nb += (*vec_it)->pdf(i);
                  *it += (*vec_it)->pdf(i);
              }
          }
      }
  }

  
  // ccdf contains the number of observations for each class.
  // Turn them into proportions
  for( it = ccdf.p_begin() ; it!= ccdf.p_end(); ++it) {
    *it = *it / replicates_nb ;
  }

  // turn the pdf into a cdf
  /*
  it = ccdf.p_begin();
  *it = *it / replicates_nb;
  ++it;
  for(; it!= ccdf.p_end(); ++it) {
    *it = *it / replicates_nb + *(it-1);
  }
  */

  // return the number of nodes droped

/*
  if ( neighbors.size()<current_level )
  {
      //GsTLcout << "simulated node id: " << u.node_id() << gstlIO::end;
      GsTLcout << "neighborhood size: " << neighbors.size() 
                       << ", neighborhood dist: " << std::distance( neighbors.begin(), neighbors.end() ) 
                       << ", max level: " << max_level
                       << ", current level: " << current_level << gstlIO::end;
      //for (typename neighborhood::const_iterator neigh_it = neighbors.begin(); neigh_it!=neighbors.end(); neigh_it++)
      //    GsTLcout << "neighborhood id: " << neigh_it->node_id() << gstlIO::end;
  }
*/
  //if ( neighbors.size() < neighbors.max_size()/4 )
  if ( neighbors.size() < 10 )
      return -neighbors.size();
  else if ( current_level<10 )
      return -current_level;
  else
      return std::distance( neighbors.begin(), neighbors.end() ) - current_level;
      //return max_level - current_level;
}
  


//------------------------------
// display 


template<class T, class allocator>
void search_tree<T, allocator>::display(std::ostream& os){
  display_all_children(os, root_);
}



template<class T, class allocator>
void search_tree<T, allocator>::display_all_children(std::ostream& os, treenode_iterator TN)
{
  os << "(level " << TN->level() << ") ";
  for(int i=0; i< nb_of_categories_; i++)
    os << TN->pdf(i) << " " ;

  os << std::endl;

  for(int i=0; i< nb_of_categories_; i++)
    if( TN->child(i) != 0 )
      {
	os << "(" << i <<")  ";
	display_all_children(os, TN->child(i));
      }
    else {
      os << "(" << i <<")  ";
      os << "end" << std::endl;
    }
}


#ifdef __GNUC__
#pragma implementation
#endif
#include "search_tree.h"
