#include <Python.h>

static PyObject* hello(PyObject* self, PyObject* args) {
    char *msg = "Hey there PyCon Portugal!";
    return Py_BuildValue("s", msg);
}

static PyMethodDef the_methods[] = {
    {"hello", (PyCFunction)hello, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name ="pyconpt",
    .m_size = 0,
    .m_methods = the_methods,
};

PyMODINIT_FUNC PyInit_pyconpt(void){
    return PyModuleDef_Init(&module);
}
