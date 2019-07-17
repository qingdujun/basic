#include <iostream>
#include <mutex>
using namespace std;
/**
 * https://www.cnblogs.com/liyuan989/p/4264889.html
 */
template <typename T>
class Singleton {
public:
    static Singleton* get() { //maybe error...because `not ctred()..`
        if (instance_ == nullptr) {
            std::lock_guard<std::mutex> locker(mutex_);
            if (instance_ == nullptr) {
                Singleton* p = new Singleton();//new and ctr()
                //TODO: barrier
                instance_ = p;
            }
        }
        return instance_;
    }

protected:
    Singleton() {
    }

private:
    static Singleton* instance_;
    static std::mutex mutex_;
};

template <typename T>
std::mutex Singleton<T>::mutex_;

template <typename T>
Singleton<T>* Singleton<T>::instance_ = nullptr;

int main(int argc, char const *argv[]) {
    Singleton<int>* ptr1 = Singleton<int>::get();
    cout << ptr1 << endl;
    Singleton<int>* ptr2 = Singleton<int>::get();
    cout << ptr2 << endl;

    return 0;
}
