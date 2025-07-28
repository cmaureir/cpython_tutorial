# Compile a C program

Consider the following snippet in C,
that prints `Hello, World!`.

```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
}
```

Save it in a file called `hello.c`.

In order to be able to execute the C code, you need to compile it first,
meaning that you will execute a command that will transform the source code
into a proper system executable.

> [!NOTE]
> This might sound like more work compared to Python, but I promise
> you will get used to it!

Go to your terminal, and run the following command where your `hello.c`
file is:

```bash
gcc hello.c -o hello_bin
```
for Linux, or replace `gcc` with `clang` for macOS.

This will create an executable called `hello_bin`.

For Windows, the command is:
```bash
cl hello.c
```
which will generate a `hello.exe` file.
