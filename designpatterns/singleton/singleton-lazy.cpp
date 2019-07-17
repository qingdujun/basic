#include <iostream>
#include <mutex>
using namespace std;

template <typename T>
class Singleton {
public:
    static Singleton* get() { //maybe error...because `local static is un..` and delete T*
        std::lock_guard<std::mutex> locker(mutex_);
        static Singleton obj;
        return &obj;
    }

protected:
    Singleton() {
    }

private:
    static std::mutex mutex_;
};

template <typename T>
std::mutex Singleton<T>::mutex_;

int main(int argc, char const *argv[]) {
    Singleton<char>* ptr1 = Singleton<char>::get();
    cout << ptr1 << endl;
    Singleton<char>* ptr2 = Singleton<char>::get();
    cout << ptr2 << endl;

    return 0;
}
