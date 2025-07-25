# Compile CPython

> ![NOTE]
> The CPython development guide provides a very detailed guide to build cpython
> for all the platforms that are supported.
>
> Feel free to check it out for more context.

Compiling Python from its source might sounds like a very complicated process,
and many people don't even try it due to that. Hopefully after this tutorial
you will manage to overcome this and do it yourself!

You learned in the previous excercise that you can compile one C-file with
a simple command line, but in the case of large projects, you need the help
of other tools that can simplify the process of compiling each file, and
creating the final executable for you.

In the case of `cpython`, we will be using a `Makefile` (for Linux and macOS)
and a custom `bat` script for Windows. Due to the process being different we
will separate the explanation.

## For Linux and macOS

Before building the project, we need to first configure it,
for that you need to run:

```
./configure
```
this shouldn't take a lot of time, between 20 seconds to a couple of minutes,
depending on your machine resources, and you will see a lot of things
happening in the terminal, but don't worry.

In case a crucial dependency is not found, you might need to install it and
re-run the command.

Once the configuration is done, you can now run the `make` command to build the
project, this might take time, so it's recommended for you to enable more CPU
cores from your system to use while building with the `-j` option. Assumming
you have 16 cores, you could run:

```
make -j 16
```

Change the number accordinly.

Once this process is over, you will see a `python` binary that you can acces
with `./python`

> !{IMPORTANT}
> the `./` is very important to indicate you want to run the
> `python` you just build, and not a system one

## For Windows

There is an *ad-hoc* script in order to build cpython, that you can use to
avoid many complicated steps. On the root of the repository, execute the
following command:

```
PCBuild\build.bat
```

You will see a lot of output, and if everything goes smoothly you should see
a message with the final executable in `PCbuild\amd64\python.exe`.
