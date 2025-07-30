# Exercise 02: Add a Python function to CPython

You probably already know that the `Lib/` directory within CPython contains all
the Python-based modules that Python includes. You are probably familiar with
many of them, so the goal will be to pick one of them, so you can add a new
one.

This might sound too complicated, but the recommendation is to pick a module
that has a function, and maybe re-write a variation for that function, by
removing one or more keyword arguments, and provide a simplified version.

You can do this with any function you want, but in case you don't know,
you can follow the example below.

## Example

Let's consider the module `glob` that contains a function with the
same name, that has the following arguments:

```py
def glob(pathname, *, root_dir=None, dir_fd=None, recursive=False,
        include_hidden=False):
```

If you are not familiar with this function, check the following example:

```py
from glob import glob

>>> glob("tutorial/**")
['tutorial/01_understanding_cpython.md', 'tutorial/02_extending_python.md', 'tutorial/exercises', 'tutorial/00_setup.md']
```

The pattern `**` means that everything found inside the `tutorial`
directory will be listed, but as you probably notice, the `tutorial/exercises`
directory content was not listed. For that, `glob` provides a way to do it
recursively, so you can call the function like this:

```py
>>> glob("tutorial/**", recursive=True)
['tutorial/', 'tutorial/01_understanding_cpython.md', 'tutorial/02_extending_python.md', 'tutorial/exercises', 'tutorial/exercises/07_helloworld_zig.md', 'tutorial/exercises/03_add_c_function.md', 'tutorial/exercises/01_compile_cpython.md', 'tutorial/exercises/04_add_container_method.md', 'tutorial/exercises/00_compile_cprogram.md', 'tutorial/exercises/02_add_python_function.md', 'tutorial/exercises/05_add_grammar_alias.md', 'tutorial/exercises/10_rewrite_python_functionality.md', 'tutorial/exercises/06_helloworld_c.md', 'tutorial/exercises/09_module_with_functions.md', 'tutorial/exercises/08_helloworld_rust.md', 'tutorial/00_setup.md']
```

To have this behavior by default, we could create `rglob` to be a variation of
`glob` that has the `recursive` parameter set to `True` by default and cannot
be changed:

```py
def rglob(pathname, *, root_dir=None, dir_fd=None, include_hidden=False):
    return glob(pathname, root_dir=root_dir, dir_fd=dir_fd, recursive=True,
    include_hidden=include_hidden)

For this particular case, we need to also modify the `__all__` variable
from:
```py
__all__ = ["glob", "iglob", "escape", "translate"]
```
to
```py
__all__ = ["glob", "rglob", "iglob", "escape", "translate"]
```

Repeating the values from the parameters when calling `glob` allows us to have
control over the behavior but only one is set to `True` which is `recursive`.

After recompiling CPython, I will be able to do:

```py
>>> from glob import rglob
>>> rglob("tutorial/**")
['tutorial/', 'tutorial/01_understanding_cpython.md', 'tutorial/02_extending_python.md', 'tutorial/exercises', 'tutorial/exercises/07_helloworld_zig.md', 'tutorial/exercises/03_add_c_function.md', 'tutorial/exercises/01_compile_cpython.md', 'tutorial/exercises/04_add_container_method.md', 'tutorial/exercises/00_compile_cprogram.md', 'tutorial/exercises/02_add_python_function.md', 'tutorial/exercises/05_add_grammar_alias.md', 'tutorial/exercises/10_rewrite_python_functionality.md', 'tutorial/exercises/06_helloworld_c.md', 'tutorial/exercises/09_module_with_functions.md', 'tutorial/exercises/08_helloworld_rust.md', 'tutorial/00_setup.md']
```


## Optional challenge

Add a new Python-based module (separate file) to `Lib/` so you can include
"more batteries" into your local Python.
