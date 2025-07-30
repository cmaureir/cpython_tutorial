# Exercise 04: Add a new container method

There are many convenient containers in Python: `list`, `tuple`, `dict`, `set`,
etc.

> [!NOTE]
> Did you know there is also a `fronzenset` ? check it out!

Many of them have convenient methods that allow us to add/remove/modify
elements within them.

The goal of this exercise is add a new one to any of your favorite containers.
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

2. Add your new definition in both sides, for example if we create a different
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

What if I want to add an element at the beginning o the list?
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

We will modify one file: `Objects/listobject.c`, and thanks to the [argument
clinic](https://devguide.python.org/development-tools/clinic/) some code
will be generated in `Objects/listobject.c.h` to enable the new method.

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

### Generating code with the argument clinic

On the `Objects/listsobject.c` file, we will add the following lines:

```c
/*[clinic input]
@critical_section
list.prepend

    object: object
    /

Prepend object to the list.
[clinic start generated code]*/
{
    if (ins1(self, 0, object) == 0) {
        Py_RETURN_NONE;
    }
    return NULL;
}
```

It might look complicated, but it's a very convenient way of writing
the implementation of our function first, and then leave the clinic
do its magic.

Notices that the section between `/* ... */` in C is a comment,
so we need to be very careful with what we put inside, because that is the
input for the argument clinic.

We specify the name of the method `list.prepend` that will accept only one
object: `object: object` and the documentation for the function: `Prepend
object to the list`.

After that, we will be skipping a function signature, but we will only add the
implementation of the function, based on the implementation of `list.insert`.

You can see we are using `ins1(self, 0, object)` while the insert
implementation has `ins1(self, index, object)`, because we know we will always
use insert with index equals to 0.

## Generating the code

Once you copied that to the file, we will run the argument clinic, manually:
```
python3 ./Tools/clinic/clinic.py --force --make --exclude Lib/test/clinic.test.c --srcdir .
```
or using make via:
```
make regen-global-objects
```

Check the modified lines in `Objects/clinic/listobject.c.h` with:
```
git diff Objects/clinic/listobject.c.h
```
and it should show you something like this:

```
PyDoc_STRVAR(list_prepend__doc__,
"prepend($self, object, /)\n"
"--\n"
"\n"
"Insert object before index.");

#define LIST_PREPEND_METHODDEF    \
    {"prepend", (PyCFunction)list_prepend, METH_O, list_prepend__doc__},

static PyObject *
list_prepend_impl(PyListObject *self, PyObject *object);

static PyObject *
list_prepend(PyObject *self, PyObject *object)
{
    PyObject *return_value = NULL;

    Py_BEGIN_CRITICAL_SECTION(self);
    return_value = list_prepend_impl((PyListObject *)self, object);
    Py_END_CRITICAL_SECTION();

    return return_value;
}
```
But even the file you edited: `Objects/listobject.c` was changed as well!
If you open it will show a couple of new lines:
```
/*[clinic input]
@critical_section
list.prepend

    object: object
    /

Insert object before index.
[clinic start generated code]*/

static PyObject *
list_prepend_impl(PyListObject *self, PyObject *object)
/*[clinic end generated code: output=1e8a17b6bb55190f input=12086cf6fd59ee03]*/
{
    if (ins1(self, 0, object) == 0) {
        Py_RETURN_NONE;
    }
    return NULL;
}
```

Did you see them? they are:
```
static PyObject *
list_prepend_impl(PyListObject *self, PyObject *object)
/*[clinic end generated code: output=1e8a17b6bb55190f input=12086cf6fd59ee03]*/
```

That was generated automatically, added a checksum and everything :)

## Compiling and trying

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
