# Compile a C program

Consider the following snippet in C,
that prints the `Hello, World!`.

```
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
}
```

save it on a file called `hello.c`.

In order to being able to execute the C code, you need to compile it first,
meaning that you will execute a command that will transform the source code
to a proper system executable.

> ![NOTE]
> This might sounds like more work compared to Python, but I promise
> you will get used to it!.

Go to your terminal, and run the following command where your `hello.c`
file is:

```
gcc hello.c -o hello_bin
```

for Linux, or replace `gcc` by `clang` for macOS.

This will create an executable called `hello_bin`.

For windows, the command is:
```
cl hello.c
```
which will generate a `hello.exe` file.
