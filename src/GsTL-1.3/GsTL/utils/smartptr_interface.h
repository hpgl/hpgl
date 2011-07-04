#ifndef __GSTL_SMARTPTR_INTERFACE_H__
#define __GSTL_SMARTPTR_INTERFACE_H__



typedef long int RefCount;


template <class T>
class SmartPtr_interface{

 public:
  SmartPtr_interface() : references_(0) {}
  RefCount references() const { return references_; }
  inline const SmartPtr_interface<T>* new_ref() const;
  inline void delete_ref() const;

 protected:
  virtual ~SmartPtr_interface() {}

 private:
  virtual void on_zero_references() { delete this; }
  RefCount references_;

}; // end of class SmartPtr_interface



template<class T>
inline 
const SmartPtr_interface<T>*
SmartPtr_interface<T>::new_ref() const {
  SmartPtr_interface<T>* me = (SmartPtr_interface<T> *) this;
  ++me->references_;
  return this;
}


template<class T>
inline 
void SmartPtr_interface<T>::delete_ref() const {
  SmartPtr_interface<T>* me = (SmartPtr_interface<T> *) this;
  if( --me->references_ == 0 )
    me->on_zero_references();
}


#endif
