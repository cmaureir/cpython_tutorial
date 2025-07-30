# Excercise 07: Minimal "Hello, World!" Zig-extension

Zig is a different programming language, so we will not have enough time to
enter into the details of it, but you will know enough to get started and
create a simple extension.

## Install the Zig compiler

Please follow [the getting started guide](https://ziglang.org/learn/getting-started/)
from the Zig language website in order to have the compiler.

You will find the instructions for macOS, Linux and Windows, with many
variations.

## Hello, World!

Add the following code into a file called `hello.zig`:

```zig
const std = @import("std");

pub fn main() !void {
    std.debug.print("Hello, World!\n", .{});
}
```
and to execute the code you can run:
```bash
zig run hello.zig
```
you can also build the code, to later run the binary manually:
```bash
$ zig build-exe hello.zig
$ ./hello
Hello, World!
```

## Bindings for Python

The project that enables Zig to be an option to write Python extensions is
called Ziggy PyDust.

The binding generator uses `poetry` to handle projects, and requires
some configuration.

First, create a `pyproject.toml` file with the following content:
```toml
[tool.poetry]
name = "hello_zig"
version = "1.0"
include = [ { path = "src/", format = "sdist" }, { path = "hello/*.so", format = "wheel" } ]
package-mode = false

[tool.poetry.build]
script = "build.py"

[build-system]
requires = ["poetry-core", "ziggy-pydust==0.25.1"]
build-backend = "poetry.core.masonry.api"

[[tool.pydust.ext_module]]
name = "hello_zig"
root = "src/hello.zig"
```

also, you need a simple `build.py` file alongside:
```py
from pydust.build import build

build()
```
and for the source code for our extension,
create a `src` directory and inside a file `hello.zig` with the content:
```zig
const py = @import("pydust");

const root = @This();

pub fn hello() !py.PyString(root) {
    return try py.PyString(root).create("Hello, World!");
}

comptime {
    py.rootmodule(root);
}
```

With all the files in place, you can run `poetry install` for later execute
the code:

```py
>>> from hello_zig import hello
>>> hello()
'Hello, World!'
```
