#ifndef __PROPERTY_TYPE_TRAITS_H__
#define __PROPERTY_TYPE_TRAITS_H__

/** This class defines a constant value that will be used to test whether a
 * property has been estimated (informed) or not: when a grid is created, its
 * property values are initialized with value "unestimated_value". The test
 * is_informed checks if the current value is equal to "unestimated_value".
 * Some types can not be compared directly with the unestimated_value: if the
 * property value is of type bool, it is not possible to define an
 * unestimated_value of type bool. This value is then of another type (e.g.
 * int) and the property value has to be cast into type
 * "unestimated_value_type".
 * The template class is specialized for the different types a property value
 * can be.
 */

template<class T>
struct property_type_traits{
  typedef T unestimated_value_type;
  static const T unestimated_value; 
};


struct property_type_traits<int>{
  typedef int unestimated_value_type;
  static const int unestimated_value=-99921175;
};

struct property_type_traits<double>{
  typedef double unestimated_value_type;
  static const double unestimated_value= -9.211175e30;
};

struct property_type_traits<float>{
  typedef float unestimated_value_type;
  static const float unestimated_value = -9.211175e30;
};


struct property_type_traits<unsigned int>{
  typedef int unestimated_value_type;  
  static const int unestimated_value = 999999345;
};

#endif
