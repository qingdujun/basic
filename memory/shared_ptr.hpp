#ifndef MOSS_MEMORY_SHARED_PTR_HPP
#define MOSS_MEMORY_SHARED_PTR_HPP

#include "unique_ptr.hpp"

template <typename T...>
class Shared_ptr{
public:
    explicit Shared_ptr(Shared_ptr*);

    Shared_ptr(Shared_ptr<T...> const&);

    inline ~Shared_ptr();

    Shared_ptr<T...>& operator=(Shared_ptr<T...> const&);

    inline T& operator*();

    inline T* operator->();

private:
    T* ptr_;
};


class CounterPolicy{
public:
    CounterPolicy();
    CounterPolicy(CounterPolicy const&);
    ~CounterPolicy();
    CounterPolicy& operator=(CounterPolicy const&);
    void init(T*);
    void dispose(T*);
    void increment(T*);
    void decrement(T*);
    bool is_zero(T*);
};

class ObjectPolicy{
public:
    ObjectPolicy();
    ObjectPolicy(CounterPolicy const&);
    ~ObjectPolicy();
    ObjectPolicy& operator=(ObjectPolicy const&);
    void dispose(T*);
};

#endif