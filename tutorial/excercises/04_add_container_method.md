# Excercise 04: Add a new container method

There are many convenient containers in Python: `list`, `tuple`, `dict`, `set`,
etc.

> [!NOTE]
> Did you know there is also a `fronzenset` ? check it out!

Many of them have convenient methods that allow us to add/remove/modify
elements within them.

The goal of this excercise is add a new one to any of your favorite containers.
To do this, I recommend you to first try to mimic a functionality of another
method but do it slightly different.

Each container type, will have two implementation files,
a header file `.h` and a source file `.c`. Check the content of the `Objects/`
directory in the cpython repository, and you will see many familiar names like
`listobject.c`, `tupleobject`, etc; but also the headers in `Objects/clinic/`
like `listobject.c.h`.

Once you have decided the one you want to modify, the steps are:

1. Find where the methods are defined:

For the `set.add` method, you will find it in `Objects/setobject.c`,
and it looks like this:
```c
static PyMethodDef set_methods[] = {
    SET_ADD_METHODDEF
    ...
```
those capital letters are a macro (think about it as an alias) to the following
code that can be found in `Objects/clinic/setobject.h.c`:
```c
#define SET_ADD_METHODDEF    \
    {"add", (PyCFunction)set_add, METH_O, set_add__doc__},
```

2. Add your new defintion in both sides, for example if we create a different
   `set.add` implementation and we call it `set.add2`:

```c
static PyMethodDef set_methods[] = {
    SET_ADD_METHODDEF
    SET_ADD2_METHODDEF
    ...
```
and
```c
#define SET_ADD2_METHODDEF    \
    {"add2", (PyCFunction)set_add2, METH_O, set_add2__doc__},
```

3. Implement the function that will perform the operation
4. Implement the documentation for the method
5. Compile cpython
6. Profit

> [!IMPORTANT]
> Take inspiration from the definition that you want to mimic so you don't need
> to write all those components from scratch

In case you have no ideas, you can do the following example.

## Example

You have probably use lists in many programs, so the method `append` might not
be something you just learned, but something you use normally to include
new elements to a list:
```py
>>> l = []
>>> l.append(42)
>>> l
[42]
>>> l.append(21)
>>> l
[42, 21]
```

What if I want to add an element at the begining o the list?
Many (if not all) of you will jump and say «Use `list.insert`!»,
which is a correct!
```py
>>> l.insert(0, 55)
>>> l
[55, 42, 21]
```
To simplify this, we could think of adding the `list.prepend` function
that does exactly what we did with the `list.insert` function,
without the need of passing the first `0` argument.

We will modify two files:
* `Objects/listobject.c`, and
* `Objects/clinic/listobject.c.h`,

It is very convenient that we can mimic what `list.insert` does,
so we don't need to implement everything by ourselves!

### Add the definition

Go to the `Objects/listobject.c` file and find where is `LIST_APPEND_METHODDEF`
added to the  `list_methods` structure, and add `LIST_PREPEND_METHODDEF`.

The could should look like this:
```c
static PyMethodDef list_methods[] = {
    {"__getitem__", list_subscript, METH_O|METH_COEXIST,
     PyDoc_STR("__getitem__($self, index, /)\n--\n\nReturn self[index].")},
    LIST___REVERSED___METHODDEF
    LIST___SIZEOF___METHODDEF
    PY_LIST_CLEAR_METHODDEF
    LIST_COPY_METHODDEF
    LIST_APPEND_METHODDEF
    LIST_PREPEND_METHODDEF
    ...
```

Now, we need to create that macro.

### Macro and implementation

Find where the LIST_APPEND_METHODDEF is in the `Objects/clinic/listobject.c.h`
file and copy the definitions, by transforming them accordenly to our new
`list.prepend` method.

For the initial macro, we will have:
```c
#define LIST_PREPEND_METHODDEF    \
    {"prepend", (PyCFunction)list_prepend, METH_O, list_prepend__doc__},
```

For the documentation, we will have:
```c
PyDoc_STRVAR(list_prepend__doc__,
"prepend($self, object, /)\n"
"--\n"
"\n"
"Prepend object to the beginning of the list.");
```

And or the implementation of the method, we will copy the functionality from
the `list.insert` method, by mimicing the `insert(0, value)` approach we tried
before.

This is the `list_insert` implementation:
```c
  static PyObject *
  list_insert(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
  {
      PyObject *return_value = NULL;
      Py_ssize_t index;
      PyObject *object;

      if (!_PyArg_CheckPositional("insert", nargs, 2, 2)) {
          goto exit;
      }
      {
          Py_ssize_t ival = -1;
          PyObject *iobj = _PyNumber_Index(args[0]);
          if (iobj != NULL) {
              ival = PyLong_AsSsize_t(iobj);
              Py_DECREF(iobj);
          }
          if (ival == -1 && PyErr_Occurred()) {
              goto exit;
          }
          index = ival;
      }
      object = args[1];
      Py_BEGIN_CRITICAL_SECTION(self);
      return_value = list_insert_impl((PyListObject *)self, index, object);  // Bingo!
      Py_END_CRITICAL_SECTION();

  exit:
      return return_value;
  }
```

where this line is what we need:
```c
return_value = list_insert_impl((PyListObject *)self, index, object);  // Bingo!
```
becuase we know the `index` in the `list.prepend` case will be `0` always.
Let's keep in mind this line, and now let's check the `list.append`
implementation:
```c
  static PyObject *
  list_append(PyObject *self, PyObject *object)
  {
      PyObject *return_value = NULL;

      Py_BEGIN_CRITICAL_SECTION(self);
      return_value = list_append_impl((PyListObject *)self, object);
      Py_END_CRITICAL_SECTION();

      return return_value;
  }
```

Can you see the line we need to replace? it's even easier if I tell you
it's the sme variable name (`return_value`).

We will copy that implementation, rename the name of the function
to `list_prepend` and replacing the critical section line with he one
we took from the `list_insert` implementation.

The final result looks like this:

```c
static PyObject *
list_prepend(PyObject *self, PyObject *object)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(self);
    return_value = list_insert_impl((PyListObject *)self, 0, object);
    Py_END_CRITICAL_SECTION();

    return return_value;
}
```

Finally, we can recompile cpython, and try it out!

```py
>>> l = [3]
>>> l.append(23)
>>> l
[2, 23]
>>> l.prepend(56)
>>> l
[56, 2, 23]
```
