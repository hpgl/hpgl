#ifndef __GSTL_OUTSIDE_REF_MANAGE_POLICY_H__
#define __GSTL_OUTSIDE_REF_MANAGE_POLICY_H__


/** This class implements non-intrusive reference counting.
 * Template parameter T is the type of the pointee.
 */

typedef long int RefCount;


template <class T>
class Outside_ref {
 public:
  Outside_ref() {
    initialize();
  }

  Outside_ref(const Outside_ref& rhs) {
    p_references_ = rhs.p_references_;
  }

  void initialize() {
    p_references_ = new RefCount;
    *p_references_ = 0;
  }
  
  inline void new_ref( T* ) const {
    ++*p_references_;
  }

  inline void delete_ref( T* ptr) const {
    if( !--*p_references_ ) {
      delete p_references_;
      on_zero_references(ptr);
    }
  }

  inline RefCount references() const { return *p_references_; }


 protected:
  virtual ~Outside_ref() {};

 private:

  // This function could be made virtual ?
  void on_zero_references(T* ptr) const { delete ptr; }

  RefCount* p_references_;

};

#endif
