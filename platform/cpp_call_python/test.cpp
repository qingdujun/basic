#include <iostream>
#include <boost/python.hpp>
// /usr/local/Cellar/python/3.7.3/Frameworks/Python.framework/Versions/3.7/include/python3.7m
using namespace std;

class Python3 {
public:
    Python3();
    ~Python3();
    void call(std::string const& cmd);
    std::string call(std::string const& cmd);

}

int main(int argc, char const *argv[]) {
    Py_Initialize();
    if (Py_IsInitialized != 0) 
        std::__throw_bad_function_call();
    


    return 0;
}
