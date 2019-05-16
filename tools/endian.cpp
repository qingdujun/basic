#include <iostream>
using namespace std;

int main() {
    int i = 0x12345678;
    if (0x78 == *(reinterpret_cast<char*>(&i))) 
        cout << "little" << endl;
    else 
        cout << "big" << endl;

    return 0;
}