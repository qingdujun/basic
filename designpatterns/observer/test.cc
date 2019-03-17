#include <iostream>
#include "observer.hpp"

struct Foo {
    void print(int a, int b) {
        std::cout << "Foo " << a + b << std::endl;
    }
};

struct Bar {
    void print(int a, int b) {
        std::cout << "Bar " << a + b  << std::endl;
    }
};

int main() {
    Events<std::function<void (int, int)>> evt;
    Foo foo;
    auto key_foo = evt.Connect( [&foo](int a, int b) { foo.print(a, b); });
    Bar bar;
    auto key_bar = evt.Connect( [&bar](int a, int b) { bar.print(a, b); });
    int a = 1, b = 2;
    evt.Notify(a, b);
    evt.Disconnect(key_foo);
    evt.Notify(a, b);

    return 0;
}