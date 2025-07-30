# Exercise 08: Minimal "Hello, World!" Rust-extension

Similarly to the last exercise, we will not have enough time to learn many
details from the Rust language, but at least the minimum to understand how to
create extensions.

## Install the Rust compiler

Please follow [the official website](https://www.rust-lang.org/tools/install)
from the Rust language website in order to have the compiler.

You will find the instructions for macOS, Linux, and Windows, with many
variations.

## Hello, World!

Add the following code into a file called `hello.rs`:

```rust
fn main() {
    println!("Hello, World!");
}
```

to compile you need to run:

``` bash
rustc hello.rs
```

and to execute it:

``` bash
./hello
```

## Bindings for Python

The project that enables Rust to be an option to write Python extensions is
called PyO3, and the binding generator uses `maturin` to handle the build process.

To start a new project, you need to run:
maturin init

This will create a starting project with a function that adds two numbers,
but for consistency, you will remove that content, and replace it with the
following code in `src/lib.rs`:

```rust
use pyo3::prelude::*;

#[pyfunction]
fn hello() -> PyResult<String> {
    Ok("Hello, World!".to_string())
}

#[pymodule]
fn hello_world_rust(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_function(wrap_pyfunction!(hello, m)?)?;
    Ok(())
}
```

Everything else is configured automatically, so you need to run:

``` bash
maturin develop
```

in order to build and install the package.

Once that process is done, you will be able to try it out like this:

```py
>>> from hello_world_rust import hello
>>> hello()
'Hello, World!'
```
