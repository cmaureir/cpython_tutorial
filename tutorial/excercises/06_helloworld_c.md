# Exercise 06: Minimal "Hello, World!" C-extension

> [!NOTE]
> There is a very good and detailed guide in the official Python documentation
> that you can check for more information.
> Check it [here](https://docs.python.org/3/extending/extending.html).

In this exercise, we will create a new Python module written in C.
This might sound very complicated, but don't be scared!

While writing the code, there are a few steps we need to follow in order to get to
a state where we can write the C-code.

Let's open a `hello.c` file and start our adventure.

## Add the base C function

The function needs to follow a few details from CPython in order to be
compatible.
To return a string, we need to convert a C-string to a Python string.

Here's the code for the first step:

```c
#include <Python.h>

static PyObject* hello(PyObject* self, PyObject* args) {
    char *msg = "Hey there PyCon Portugal!";
    return Py_BuildValue("s", msg);
}
```
We still have a few steps before we can compile it!

## Add the methods definition

Because our module will have only one function, we will add only that one, and
then add a couple of *C-None* called NULL to identify when the definition
finishes.

Here's the methods definitions:

```c
static PyMethodDef the_methods[] = {
    {"hello", (PyCFunction)hello, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};
```

Notice that we have a few details that we will not explore too much like
casting the function as a `PyCFunction` or setting that the function doesn't
have any arguments with `METH_NOARGS`.

## Add the module definition

Before initializing the module, we need to define the structure with a few
placeholder elements, but the most important part is to pass the module name
and also the methods definitions:

```c
static struct PyModuleDef module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "pyconpt",
    .m_size = 0,
    .m_methods = the_methods
};
```

## Initialize the module

For the last step from the C-side of things, we need to initialize the module
in order to create the module.

```c
PyMODINIT_FUNC PyInit_pyconpt(void){
    return PyModuleDef_Init(&module);
}
```

## Creating a package (wheel)

Create a `pyproject.toml` alongside the `hello.c` file, with the following
content:

```toml
[build-system]
requires = ["setuptools"]
build-backend = "setuptools.build_meta"

[project]
name = "pyconpt"
version = "1.0"

[tool.setuptools]
ext-modules = [
  {name = "pyconpt", sources = ["hello.c"]}
]
```

Install the `build` and `setuptools` Python packages, and then run:

```bash
python -m build -w
```

> [!IMPORTANT]
> Make sure the `python` executable is the one that has setuptools and build
> installed.

If everything succeeds, you will have a `.whl` package in the `dist/` directory
that you can install, to later execute like:

```py
>>> from pyconpt import hello
>>> hello()
"Hey there PyCon Portugal!"
```

> [!OPTIONAL]
> What if you would like to do this in C++?
> Check out `std::string` as a type and replace the content of the hello
> function
