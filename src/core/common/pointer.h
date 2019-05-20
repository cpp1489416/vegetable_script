// "Copyright [year] <Copyright Owner>"

#ifndef VEGETABLE_SCRIPT_CORE_UTIL_POINTER_H_
#define VEGETABLE_SCRIPT_CORE_UTIL_POINTER_H_

namespace vegetable_script {

// this pointer is a simple type of shared_ptr
template<typename T>
class Pointer {
 public:
  Pointer(): pointer_(nullptr) {
    count_ = new int(1);
  }

  explicit Pointer(T* p): pointer_(p) {
    count_ = new int(0);
    ++(*count_);
  }

  Pointer(const Pointer<T>& rhs): pointer_(rhs.pointer_), count_(rhs.count_) {
    ++(*count_);
  }

  ~Pointer() {
    --(*count_);
    if (count_ == 0) {
      delete pointer_;
      delete count_;
    }
  }

  const T& operator*() const { return *pointer_; }

  T& operator*() { return *pointer_; }

  const T* operator->() const { return pointer_; }

  T* operator->() { return pointer_; }

  Pointer<T> operator=(const Pointer<T>& rhs) {
    if (&rhs != this) {
      if (--(*count_) == 0) {
        delete pointer_;
        delete count_;
      }
      pointer_ = rhs.pointer_;
      count_ = rhs.count_;
      ++(*count_);
    }
    return *this;
  }

 private:
  T* pointer_;
  int* count_;
};

}  // namespace vegetable_script

#endif  //  VEGETABLE_SCRIPT_CORE_UTIL_POINTER_H_
