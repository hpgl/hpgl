
template 
  < 
     class T,
     class RefManagePolicy
  >
SmartPtr<T,RefManagePolicy>::SmartPtr( T* ptr ) 
  : RefManagePolicy(), raw_ptr_(ptr) {
  
  if( raw_ptr_ ) 
    RefManagePolicy::new_ref(raw_ptr_);
}



template 
  < 
     class T,
     class RefManagePolicy
  >
SmartPtr<T,RefManagePolicy>::SmartPtr( const SmartPtr<T, RefManagePolicy>& smart_ptr )
  : RefManagePolicy(smart_ptr), raw_ptr_( smart_ptr.raw_ptr() ) {
  if( raw_ptr_ ) 
    new_ref(raw_ptr_);
}


template 
  < 
     class T,
     class RefManagePolicy
  >
SmartPtr<T,RefManagePolicy>::~SmartPtr() {
  if( raw_ptr_ ) 
    RefManagePolicy::delete_ref(raw_ptr_);
}



template 
  < 
     class T,
     class RefManagePolicy
  >
void 
SmartPtr<T,RefManagePolicy>::
operator=( const SmartPtr<T, RefManagePolicy>& smart_ptr ) {

  T* save = raw_ptr_;
  raw_ptr_ = smart_ptr.raw_ptr_;
  if( raw_ptr_ )
    new_ref(raw_ptr_);
  if( save )
    delete_ref(raw_ptr_);
}


#include <GsTL/utils/smartptr.h>
