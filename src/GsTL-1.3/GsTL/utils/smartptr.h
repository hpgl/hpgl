#ifndef __GSTL_UTILS_SMARTPTR_H__
#define __GSTL_UTILS_SMARTPTR_H__


#include <GsTL/utils/intru_ref_manage_policy.h>


/** This class implements smart pointers with reference counting.
 * There are at least two ways to implement reference counting:
 *   - the reference is stored inside the pointee object. This technique
 *     is refered to as intrusive reference counting. 
 *   - the reference is stored outside the pointee object. 
 * This smart pointer supports both mechanism, and its default behavior is
 * intrusive reference counting. For enable an object for intrusive reference
 * counting, it must derive from SmartPtr_interface<T>. 
 * The type of reference counting is selected by template argument 
 * RefManagePolicy. RefManagePolicy can be either:
 *   - Intrusive_ref_management<T>, for intrusive reference counting, or 
 *   - Outside_ref<T>, for non-intrusive reference counting.
 * Each time, T is the type of the pointee
 * 
 * Credits: implementation ideas were taken from: 
 *   - David R. Cheriton's textbook: "Object Oriented Programming from a 
 *     Modeling and Simulation Perspective" (this textbook is used in
 *     class CS249 at Stanford University, CA).
 *   - Andrei Alexandrescu, "Modern C++ Design", Addison Wesley, 2001
 */



template 
  < 
     class T,
     class RefManagePolicy = Intrusive_ref_management<T>
  >
class SmartPtr : public RefManagePolicy {
  
  public:
  
  typedef T pointee_type;

  SmartPtr( T* ptr = 0 );
  SmartPtr( const SmartPtr<T,RefManagePolicy>& smart_ptr);

  ~SmartPtr();

  void operator=(const SmartPtr<T,RefManagePolicy>& smart_ptr);

  bool peer(const SmartPtr<T,RefManagePolicy>& smart_ptr) const {
    return (raw_ptr_ == smart_ptr.raw_ptr_);
  }
  
  bool operator==(const SmartPtr<T,RefManagePolicy>& smart_ptr) const {
    return peer(smart_ptr);
  }
  bool operator!=(const SmartPtr<T,RefManagePolicy>& smart_ptr) const {
   return !peer(smart_ptr);
  }
 
  const T* operator->() const { return raw_ptr_; }
  T* operator->() { return raw_ptr_; }
  
  T* raw_ptr() const {return raw_ptr_;}

  //=======================
  // Conversions 

  // conversion to bool
  operator bool() const {return raw_ptr_ ? 1: 0 ;}

  // conversion to another type of smart pointer
  template <class OtherType>
  operator SmartPtr<OtherType,RefManagePolicy>() {
    return SmartPtr<OtherType,RefManagePolicy>( raw_ptr_ );
  }


  private:

  T* raw_ptr_;
  
};


#include "smartptr.cc"

#endif
