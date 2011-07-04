#ifndef __GSTL_PROPERTY_SERVER_H__
#define __GSTL_PROPERTY_SERVER_H__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/grid/property_array.h>


#include <vector>
#include <map>
#include <string>
#include <algorithm>

#include <cassert>




/** A property server is a database of values of different properties.
 * In this implementation, all the properties are of the same type,
 * and they all contain the same number of values.
 * Each property has a name, and to each name is associated an 
 * integer id.
 * The user only has to supply the names. The id are consecutive 
 * integers.
 */

template<class T>
class property_server{

 public:
  typedef T property_type;
  typedef property_array<property_type> property_array_type;
  

  property_server(int nb_of_nodes, int nb_of_properties=0);

  /** Constructs a property server, with nb_of_properties properties.
   * The property names are given by array "names" and the property values
   * by the array of property_array. The property values are copied from
   * "properties". 
   */ 
  property_server(int nb_of_properties, 
		  std::string* names,
		  property_array_type** properties);

  ~property_server();

  /** @return false if the function failed.
   * @param name is the name of the property to be created
   */
  bool new_property(std::string name);
  bool new_property(std::string name, property_type initial_value);

  /** Creates a new property called "name" and initializes it with
   * the values contained in property (deep copy).
   */
  bool new_property(std::string name, const property_array_type* property);

  /** This function deletes property "name". This function is not optimized
   * for speed as it is not expected that this function would be often called.
   * In particular, if the number of properties is large, this function can
   * be time-consuming.
   * @return false if the property could not be deleted (e.g. it does
   * not exist).
   */  
  bool delete_property(std::string name);
  bool delete_property(int name_id);


  /** @return true if property "name" exists in the database.
   */
  inline bool property_exists(std::string name) const ;
  inline bool property_exists(int name_id) const ;

  /** Access function. 
   * This function will fail if there is no property called "name".
   * In such a case, the function simply aborts. 
   * @param index is the index of the node at which the property value
   * is to be retrieved
   * @param name is the name of the property of interest.
   */

  inline property_type& property_value(int index, std::string name);
  inline const property_type& property_value(int index, std::string name) const;

  inline property_type& property_value(int index, int name_id=0);
  inline const property_type& property_value(int index, int name_id=0) const;

 
  /** This function provides access to the whole property_array, for
   * a given property. 
   */
  inline property_array_type* get_property_array(std::string name);
  inline property_array_type* get_property_array(int name_id);
  

  /** @return the name of the property of id name_id
   */
  std::string property_name(int name_id) const;

  /** @return the name_id of property "name"
   */
  inline int property_name_id(std::string name) const;

  /** @return the number of values per property.
   */
  inline int size() const;

  inline int nb_of_properties() const;



 private:
  
  int nb_of_nodes_;
  std::vector<property_array_type*> properties_;
  std::map<std::string,int> property_name_map_;

  typedef std::map<std::string,int>::iterator map_iterator;
  typedef std::map<std::string,int>::const_iterator map_const_iterator;



  // forbid use of copy constructor
  property_server(const property_server& rhs);

  // forbid use of assignment operator
  property_server& operator=(const property_server& rhs);

}; // end of class property_server






//=====================================
// Inline functions

template<class T>
inline bool property_server<T>::property_exists(std::string name) const {
  return property_name_map_.find(name) != property_name_map_.end();
}



template<class T>
inline bool property_server<T>::property_exists(int name_id) const {
  return static_cast<int>( properties_.size() ) > name_id;
}





template<class T>
inline typename property_server<T>::property_type&
property_server<T>::property_value(int index, std::string name) {
  map_iterator it = property_name_map_.find(name);

  assert(it != property_name_map_.end() );
  
  return (*properties_[it->second])[index];
}




template<class T>
inline const typename property_server<T>::property_type&
property_server<T>::property_value(int index, std::string name) const {
  map_const_iterator it = property_name_map_.find(name);

  assert(it != property_name_map_.end() );
  return (*properties_[it->second])[index];
}




template<class T>
inline typename property_server<T>::property_type&
property_server<T>::property_value(int index, int name_id) {
  assert( property_exists(name_id) ); 
  return (*properties_[name_id])[index];
}


template<class T>
inline const typename property_server<T>::property_type&
property_server<T>::property_value(int index, int name_id) const {
  assert( property_exists(name_id) ); 
  return (*properties_[name_id])[index];
}



template<class T>
inline typename property_server<T>::property_array_type*
property_server<T>::get_property_array(std::string name) {
  map_iterator it = property_name_map_.find(name);

  assert(it != property_name_map_.end() );
  return properties_[it->second];
}



template<class T>
inline typename property_server<T>::property_array_type*
property_server<T>::get_property_array(int name_id) {
  assert( property_exists(name_id) );
  return properties_[name_id];
}



template<class T>
inline int property_server<T>::property_name_id(std::string name) const {
  map_const_iterator it = property_name_map_.find(name);
  assert(it != property_name_map_.end());
  return it->second;  
}



template<class T>
inline int property_server<T>::size() const {
  if(properties_.size() != 0)
    return properties_[0]->size();
  else
    return 0;
}


template<class T>
inline int property_server<T>::nb_of_properties() const {
  return properties_.size();
}






#if defined(__GNUC__) || defined(WIN32)
#include "property_server.cc"
#elif defined(__xlC__)
#pragma implementation("property_server.cc")
#endif

#endif
