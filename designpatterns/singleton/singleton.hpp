#ifndef MOSS_DESIGN_PATTERNS_SINGLETO_SINGLETON_HPP
#define MOSS_DESIGN_PATTERNS_SINGLETO_SINGLETON_HPP

#include <utility> //std::forward

template <typename T>
class Singleton {
public:
    template <typename... N>
    static void Instance(N&&... args) {
        if (instance_ == nullptr) 
            instance_ = new T(std::forward<N>(args)...);
    }

    static T* GetInstance() {
        if (instance_ == nullptr) 
            throw std::logic_error("instance_ == nullptr");
        return instance_;
    }

    static void DestroyInstance() {
        delete instance_;
        instance_ = nullptr;
    }
private:
    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    static T* instance_;
};

template <class T> T* Singleton<T>::instance_ = nullptr;
#endif