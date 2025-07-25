# Excercise 03: Add a C-function to CPython built-ins

This excercise can be a bit tricky for the people that is not too familiarized
with C, but with the help of the Python C-API the implementation can be a bit
simpler.

The goal of this excercise, is to experience the structure and details for all
the C components within Python, so take your time when trying to learn this.

You can try any file from the `Modules/` directory, but also you can find
a few functions in the `Python/` directory that contains the runtime.

## Example

As an example, we will look into a function that might be familiar to you, the
`dir()` function.

In case you don't know what the built-in function does, it will list all the
members of a variable type. In the case of an integer:

```
>>> i = 42
>>> dir(i)
['__abs__', '__add__', '__and__', '__bool__', '__ceil__', '__class__', '__delattr__', '__dir__', '__divmod__', '__doc__', '__eq__', '__float__', '__floor__', '__floordiv__', '__format__', '__ge__', '__getattribute__', '__getnewargs__', '__getstate__', '__gt__', '__hash__', '__index__', '__init__', '__init_subclass__', '__int__', '__invert__', '__le__', '__lshift__', '__lt__', '__mod__', '__mul__', '__ne__', '__neg__', '__new__', '__or__', '__pos__', '__pow__', '__radd__', '__rand__', '__rdivmod__', '__reduce__', '__reduce_ex__', '__repr__', '__rfloordiv__', '__rlshift__', '__rmod__', '__rmul__', '__ror__', '__round__', '__rpow__', '__rrshift__', '__rshift__', '__rsub__', '__rtruediv__', '__rxor__', '__setattr__', '__sizeof__', '__str__', '__sub__', '__subclasshook__', '__truediv__', '__trunc__', '__xor__', 'as_integer_ratio', 'bit_count', 'bit_length', 'conjugate', 'denominator', 'from_bytes', 'imag', 'is_integer', 'numerator', 'real', 'to_bytes']
```
where there are many useful functions for `int` objects.
Similarly with another type, a Python list:

```
>>> l = []
>>> dir(l)
['__add__', '__class__', '__class_getitem__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__getstate__', '__gt__', '__hash__', '__iadd__', '__imul__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__mul__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__reversed__', '__rmul__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 'append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
```

In some cases, is difficult to see all the names of the public methods
because a lot of dunder ones get in the middle, so what you could do to see
clear? Maybe something like this:

```
>>> l = []
>>> [i for i in dir(l) if "__" not in i]
['append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
```

where you can clearly see the usual methods people use with their lists.
The example challenge is to build a function, **in C** that does that for us,
for simplicity we will call it `pdir()`.

### Where is the function?

If you search the code, you will find that the declaration of the `dir`
function is in the `Python/bltinmodule.c` file, if you scroll a bit you will
find a declaration of `builtin_methods`, and the line looks like this:
```
static PyMethodDef builtin_methods[] = {
```

Inside that, you will find the definition of the `dir` function:
```
    {"dir", builtin_dir, METH_VARARGS, dir_doc},
```
here we have a few interesting components:

* `"dir"`, is the name we want to expose to Python,
* `builtin_dir` is the function that implements the functionality,
* `METH_VARARGS` is the structure for the function arguments (we don't need to
worry much about this, but you can read more
[here](https://docs.python.org/3/c-api/structures.html#c.METH_VARARGS),
* `dir_doc`, is the documentation you see when doing `help(dir)`.

Look at the implementation of the `dir` function:
```
static PyObject *
builtin_dir(PyObject *self, PyObject *args)
{
    PyObject *arg = NULL;

    if (!PyArg_UnpackTuple(args, "dir", 0, 1, &arg))
        return NULL;
    return PyObject_Dir(arg);
}
```

It looks like, the important part that get the information
is the line `return PyObject_Dir(arg);`, because the other one is related to
the arguments, so we can use the same approach we used before:

```
>>> l = []
>>> [i for i in dir(l) if "__" not in i]
```
but from C, and using some of Python's C-API.

### Writing Python code that looks closer to C

If we write that comprehension more explicitely, and incorporating
a few C-isms so it's easier for us to write the C code.

```
l = []

pdir_list = []
dir_list = dir(l)

for i in range(len(dir_list)):
    item = dir_list[i]
    if item[0] == "_" and item[1] == "_":
        continue
    pdir_list.append(item)

return pdir_list
```
I know you might be thinking that things could be written very differently, but
once you look into the C implementation, you will understand!

### C-API and concepts to remember

To prepare ourselves, let's write down the equivalents of the C-API
so we can re-write it to C:

* `PyList_New`, to create a list,
* `PyList_Size`, to get the lenght of a list,
* `PyList_GetItem`, to get an item in a specific position.

Keep also in mind that Python objects will be `PyObject`,
that for loops in C can be written:
```
for (int index = 0; i < 10; i++)
```
which means that it's a for-loop that goes from 0 to 9.

Another important detail, is that to transform a Python string (from C),
to a C-string, one needs to use a specific function called `PyUnicode_AsUTF8`.

### The C code

Remember we are starting from the `builtin_dir` implementation

```
static PyObject *
builtin_dir(PyObject *self, PyObject *args)
{
    PyObject *arg = NULL;

    if (!PyArg_UnpackTuple(args, "dir", 0, 1, &arg))
        return NULL;
    return PyObject_Dir(arg); // We need to filter this
}
```

so more or less the structure should be:

```
static PyObject *
builtin_dir(PyObject *self, PyObject *args)
{
    PyObject *arg = NULL;

    if (!PyArg_UnpackTuple(args, "dir", 0, 1, &arg))
        return NULL;

    PyObject *dir_list = PyObject_Dir(arg);
    // filter the content here
    return pdir_list;
}
```

If we put everything together, we can re-write our Python code to:


```
static PyObject *
builtin_pdir(PyObject *self, PyObject *args)
{
    PyObject *arg = NULL;

    if (!PyArg_UnpackTuple(args, "dir", 0, 1, &arg))
        return NULL;

    // Create empty list
    PyObject *pdir_list = PyList_New(0);

    // Get elements from the dir function
    PyObject *dir_list = PyObject_Dir(arg);

    // Iteration over dir_list
    for (Py_ssize_t i = 0 ; i < PyList_Size(dir_list); i++) {

       // Get the i-item from the dir_list
       PyObject *item = PyList_GetItem(dir_list, i);

       // Transform the Python string to a C string, and compare
       const char *item_chars = PyUnicode_AsUTF8(item);
       if (item_chars[0] == '_' && item_chars[1] == '_')
           continue;

       // Calling a list append, but checking no error happened
       if (PyList_Append(pdir_list, item) != 0)
           return NULL;

       Py_DECREF(item); // References are for another day.
    }
    Py_DECREF(dir_list);
    A
    // Returned the filtered list
    return pdir_list;
}
```

Now, we can recompile cpython, and you will be able to have:

```
>>> l = []
>>> dir(l)
['__add__', '__class__', '__class_getitem__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__getstate__', '__gt__', '__hash__', '__iadd__', '__imul__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__mul__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__reversed__', '__rmul__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 'append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
>>> pdir(l)
['append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']
```

> ![WARNING]
> The temptation of making this a real contribution might be strong in you,
> but don't do it :P
