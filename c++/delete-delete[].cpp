#include <iostream>
using namespace std;

/**
 * https://www.jianshu.com/p/fbf6cad613e9
 * https://www.zhihu.com/question/30838092
 */
class Base {
public:
    virtual ~Base() { cout << "Good bye, Base!" << endl; }
};

class Derived : public Base {
public:
    virtual ~Derived() { cout << "Good bye, Derived!" << endl; }
};

int main(int argc, char const* argv[]) {
    int* ptr1 = new int[8];
    int* ptr2 = new int[8];  // 0x30-0x10=0x20=32d

    cout << ptr1 << " " << ptr2 << " " << sizeof(int) << " " << sizeof(size_t) << endl;
    // 0x7f8bb9402810 0x7f8bb9402830 4 8
    Base* base1 = new Derived();
    delete base1;

    // Base* base2 = new Derived[8];
    // delete base2;//pointer being freed was not allocated

    Base* base2 = new Derived[8];
    delete[] base2;//offset sizeof(Base)

    return 0;
}

/** 
MacBook-Pro:c++ qingdujun$ ./a.out 
0x7fa0cfc02810 0x7fa0cfc02830 4 8
Good bye, Derived!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
Good bye, Base!
**/