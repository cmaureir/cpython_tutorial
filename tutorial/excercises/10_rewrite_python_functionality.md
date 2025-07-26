# Excercise 10: Re-write Python functionality in Rust

This is the final test.
All the effort you put in the previous excercises will pay off in this step.

With the growing popularity from Rust, you need to think of one of the
functions that Python includes which is written in Python (`Lib/` directory)
and you will re-write in in Rust.

You don't need to do a full-module, but only one specific functionality.

In case you don't have an idea, you can follow the example.

## Example

We will re-write the function `glob.glob` in order to get a function that allow
us to get the files from a directory, recursively or not.

Check the `glob` signature:

```py
def glob(pathname, *, root_dir=None, dir_fd=None, recursive=False,
        include_hidden=False):
``

We will not cover all the arguments, but we will focus only on the
signature:

```py
def glob(pathname, *, recursive=False):
``

The structure for the PyO3 module is given by `maturin init` so your job is to
write the content of the module.

Rust has a module you can use called `walkdir::WalkDir` for you to consider the
recursive case, and also the `std::fs` for the non recursive case.

The logic of the function is that depending on the value of the `recursive`
variable, you will use one or the other.

Remember that you can create a list by doing:
```rust
let list: &PyList = PyList::empty(...);
```
and you can use the `.append` method on it.

The final code including the used modules, the function and the module
initialization is shown bellow:


```rust
use pyo3::{prelude::*, types::PyList};
use std::fs;
use walkdir::WalkDir;


#[pyfunction]
fn glob(py: Python<'_>, path: String, recursive: Option<bool>) -> PyResult<&PyList> {
    let list: &PyList = PyList::empty(py);
    if recursive.unwrap_or(false) == false {
        for path in fs::read_dir(path).unwrap() {
            let _ = list.append(path.unwrap().path().display().to_string());
        }
    } else {
        for path in WalkDir::new(path) {
            let _ = list.append(path.unwrap().path().display().to_string());
        }
    };
    Ok(list)
}

#[pymodule]
fn rust_glob(_py: Python, m: &PyModule) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(glob, m)?)?;
    Ok(())
}

```
