namespace std2 {
/**
 * https://www.boost.org/doc/libs/1_53_0/boost/noncopyable.hpp
 */
class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

} //namespace std2