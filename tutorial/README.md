# Setup

There is a very detailed guide to get started with CPython
from the [official documentation](https://devguide.python.org/getting-started/setup-building/)
that can provide more context and details on each step.

To simplify the process, make sure you have the following dependencies
in your computer:

* Git (optional, but recommended)
* A C-compiler:
    * `gcc` for Linux,
    * `clang` for macOS (via XCode),
    * `cl` for Windows (via MSVC)
* Your favorite editor or IDE.

It's recommended that you clone the `cpython` repository
so you can access the code that we will use:

```
git clone https://github.com/python/cpython
```
in case your Internet is not so good, you can do a clone of the last
commit from the repository (also called *shallow clone*) by using:

```
git clone --depth=1 https://github.com/python/cpython
```

You are now prepared to start the tutorial!

## What's next?

There are two main sections on this tutorial,
each one will be followed by a couple of exercises, which you can find in the
[exercises directory](exercises/).

You can access them directly from here:
* [Compile a C Program](exercises/00_compile_cprogram.md)
* [Compile CPython](exercises/01_compile_cpython.md)
* [Add a Python function to CPython](exercises/02_add_python_function.md)
* [Add a C function to CPython](exercises/03_add_c_function.md)
* [Add a new method to a Python container](exercises/04_add_container_method.md)
* [Add a new grammar alias to CPython](exercises/05_add_grammar_alias.md)
* [A 'Hello, World!' extension in C](exercises/06_helloworld_c.md)
* [A 'Hello, World!' extension in Zig](exercises/07_helloworld_zig.md)
* [A 'Hello, World!' extension in Rust](exercises/08_helloworld_rust.md)
* [Create an extension with a couple of function](exercises/09_module_with_functions.md)
* [Re-write a Python function in Rust](exercises/10_rewrite_python_functionality.md)
