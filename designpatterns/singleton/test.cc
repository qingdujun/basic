#include <iostream>
#include <string>
#include "singleton.hpp"

struct Foo {
    Foo(const std::string& val) {
        std::cout << val << std::endl;
    }

    ~Foo() {
        std::cout << "~Foo()" << std::endl;
    }
};

int main() {
    Singleton<Foo>::Instance("aaaaa");
    Singleton<Foo>::Instance("bbbbb");
    Singleton<Foo>::DestroyInstance();
    Singleton<Foo>::Instance("ccccc");
    Singleton<Foo>::DestroyInstance();

    return 0;
}