#include <boost/python.hpp>
// /usr/local/Cellar/python/3.7.3/Frameworks/Python.framework/Versions/3.7/include/python3.7m

class Python3 {
public:
    Python3();
    ~Python3();
    bool IsInitialized();
    std::string Call(char const* str);
}

Python3::Python3() {
    Py_Initialize();
}

~Python3::Python3() {
    Py_Finalize();
    Py_IsInitialized
}

Python3::IsInitialized() {
    return Py_IsInitialized == 0;
}

std::string Call(std::string const& str, ) {
    PyObject* model_ptr = nullptr;
    PyObject* func_ptr = nullptr;
    model_ptr = PyImport_Import(PyString_FromString(cmd));
    func_ptr = PyObject_GetAttrString(model_ptr, );
    PyObject_Get
}