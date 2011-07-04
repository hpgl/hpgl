#include "property_array.h"

#include <sstream>


template<class T>
property_server<T>::property_server(int nb_of_nodes, int nb_of_properties)
  : nb_of_nodes_(nb_of_nodes) {
 
  for(int i=0; i<nb_of_properties ; i++) {
    property_array_type* property = new property_array_type(nb_of_nodes);
    properties_.push_back(property);
    
    // give the property a default name: "default_i"
    std::ostringstream to_string;
    to_string << i;
    std::string name = "default_"+to_string.str();
    property_name_map_[name]=i;
  }
}



template<class T>
property_server<T>::property_server(int nb_of_properties, 
				    std::string* names,
				    property_array_type** properties) {
  
  nb_of_nodes_ = properties[0]->size();

  for(int i=0; i<nb_of_properties ; i++) {
    property_name_map_[ names[i] ] = i;

    property_array_type* property = new property_array_type(nb_of_nodes);
    properties_. push_back(property);
    std::copy(properties[i]->begin(), properties[i]->end(), property->begin() );
  }
}



template<class T>
property_server<T>::~property_server() {
  typedef typename std::vector<property_array_type*>::iterator iterator;
  for(iterator it=properties_.begin() ; it!=properties_.end() ; it++) {
    if(*it != 0) {
      delete *it;
      *it=0;
    }
  }
}




template<class T>
bool property_server<T>::new_property(std::string name) {
  //check property "name" does not already exist
  if(property_name_map_.find(name) != property_name_map_.end() )
    return false;

  //create new property array and store it.
  property_name_map_[name] = properties_.size();
  property_array_type* new_prop = new property_array_type(nb_of_nodes_);

  properties_.push_back(new_prop);

  return true;
}


template<class T>
bool property_server<T>::new_property(std::string name,
				      property_type initial_value) {

  //check property "name" does not already exist
  if(property_name_map_.find(name) != property_name_map_.end() )
    return false;

  //create new property array and store it.
  property_name_map_[name] = properties_.size();
  property_array_type* new_prop = new property_array_type(nb_of_nodes_);

  properties_.push_back(new_prop);

  // initialize property array
  for(typename property_array_type::iterator it=new_prop->begin(); 
      it!=new_prop->end(); ++it)
    *it = initial_value;

  return true;
}


template<class T>
bool property_server<T>::new_property(std::string name, 
				      const property_array_type* property) {
  //check property "name" does not already exist
  if(property_name_map_.find(name) != property_name_map_.end() )
    return false;

  property_name_map_[name] = properties_.size();
  property_array_type* new_prop = new property_array_type(nb_of_nodes_);

  properties_.push_back(new_prop);

  //copy the property values
  std::copy(property->begin(), property->end(), new_prop->begin());
  return true;
}



template<class T>
bool property_server<T>::delete_property(std::string name) {
  if(property_name_map_.find(name) == property_name_map_.end() )
    return false;

  // delete property
  int index = property_name_map_[name];
  delete properties_[index];

  properties_.erase( properties_.begin()+index );

  // re-compute indexes. All the indexes are consecutive numbers:
  // if there are 4 properties and property 2 is deleted, then the new
  // indexes are 1,2,3, not 1,3,4.
  property_name_map_.erase(name);
  for(map_iterator it=property_name_map_.begin(); it!= property_name_map_.end(); it++)
    {
      if(it->second > index)
	it->second--;	
    }
  return true;
}





template<class T>
bool property_server<T>::delete_property(int name_id) {
  if(name_id >= properties_.size() )
    return false;

  // delete property
  int index = name_id;
  delete properties_[index];

  properties_.erase( properties_.begin()+index );

  // re-compute indexes. All the indexes are consecutive numbers:
  // if there are 4 properties and property 2 is deleted, then the new
  // indexes are 1,2,3, not 1,3,4.
  for(map_iterator it=property_name_map_.begin(); it!= property_name_map_.end(); it++)
    {
      if(it->second > index)
	it->second--;	
      if(it->second == index)
	property_name_map_.erase(it);
    }
  return true;
}






template<class T>
std::string property_server<T>::property_name(int name_id) const {
  assert( property_exists(name_id) );
  map_const_iterator it = property_name_map_.begin();

  bool found = false;

  while( it!= property_name_map_.end() && !found ) {
    if(it->second == name_id)
      found = true;
    else
      ++it;
  }
  return it->first;
}



#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/grid/property_server.h>
