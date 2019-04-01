#ifndef MOSS_MEMORY_UNIQUE_PTR_HPP
#define MOSS_MEMORY_UNIQUE_PTR_HPP

template <typename T>
class Unique_ptr {
public:
    Unique_ptr() : ptr_(nullptr) {

    }

    explicit Unique_ptr(T* ptr) : ptr_(ptr) {

    }

    Unique_ptr(Unique_ptr<T> const&) = delete;

    Unique_ptr<T>& operator=(Unique_ptr<T> const&) = delete;

    ~Unique_ptr() {
        delete ptr_;
    }

    Unique_ptr<T>& operator=(T* ptr) {
        delete ptr_;
        ptr_ = ptr;
        return *this;
    }

    T& operator*() const {
        return *this;
    }

    T* operator->() const {
        return ptr_;
    }

    T* get() const {
        return ptr_;
    }

    T* release() {
        T* ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }

    void exchange_with(Unique_ptr<T>& ref) {
        std::swap(ptr_, ref.ptr_);
    }

    void exchange_with(T*& ptr) {
        std::swap(ptr_, ptr);
    }
private:
    T* ptr_;
};

#endif