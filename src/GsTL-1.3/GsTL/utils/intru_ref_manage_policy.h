#ifndef __GSTL_INTRU_REF_MANAGE_POLICY_H__
#define __GSTL_INTRU_REF_MANAGE_POLICY_H__



#include <GsTL/utils/smartptr_interface.h>

/** This class implements the intrusive reference management policy. 
 * T is the type of the pointee.
 * T must provide functions new_ref() and delete_ref().
 * Practically, T should be a SmartPtr_interface<..> or a Named_interface<..>
 */

template<class T>
struct Intrusive_ref_management {

  //  inline T* new_ref(T* ptr) const {
  //    return ptr->new_ref();
  //  }

  inline void new_ref(T* ptr) const {
    ptr->new_ref();
  }

  inline void delete_ref(T* ptr) const {
    ptr->delete_ref();
  }

};



#endif
