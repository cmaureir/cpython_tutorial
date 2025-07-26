# Excercise 05: Add Grammar alias

Python's grammar can be a bit complicated to understand,
so that's why adding new elements to it, can become a complicated task.
On this excercise is not to create something completely new, but to adapt some
existing logic from the grammar, to new elements.

You are familiarized with keyword `if`, you know it's the basic building block
to check the *truthness* of an expression, like `a == 1` or `b != d`.

So what if we could add a new keyword with the same functionality?

Maybe in your native language (for non-native English speakers) the word `if`
can be written differently, like in Portuguese `se` or Spanish `si`,
so the goal of this excercise is to include this new grammar, as an alias to
the `if` functionality.

> [!IMPORTANT]
> If your native language word for `if` contains non-ASCII characters, this
> will not work due to some cpython limitations of the encoding of keyword.
> Further changes in the parsing of the content is required in order to make it
> work.

If you don't know where to start, you need to see the `Grammar/python.gram`
file in the cpython repository, and copy the `if_stmt` grammar rules:

```
if_stmt[stmt_ty]:
    | invalid_if_stmt
    | 'if' a=named_expression ':' b=block c=elif_stmt {
        _PyAST_If(a, b, CHECK(asdl_stmt_seq*, _PyPegen_singleton_seq(p, c)), EXTRA) }
    | 'if' a=named_expression ':' b=block c=[else_block] { _PyAST_If(a, b, c, EXTRA) }
```
but also add a new line where the `compound_stmt` are declared, as a new line
under the `if_stmt`.
```
compound_stmt[stmt_ty]:
    | &('def' | '@' | 'async') function_def
    | &'if' if_stmt
    ...
```

For the sae of this situation, we will use the spanish word for it: `si`.

I will need to create a new entry in the `compound_stmt` declaration,
and a new statement, so in the file I will now have:


```
compound_stmt[stmt_ty]:
    | &('def' | '@' | 'async') function_def
    | &'if' if_stmt
    | &'si' si_stmt
    ...
```
and also
```
si_stmt[stmt_ty]:
    | invalid_if_stmt
    | 'si' a=named_expression ':' b=block c=elif_stmt {
        _PyAST_If(a, b, CHECK(asdl_stmt_seq*, _PyPegen_singleton_seq(p, c)), EXTRA) }
    | 'si' a=named_expression ':' b=block c=[else_block] { _PyAST_If(a, b, c, EXTRA) }
```

Please notice because we are mimicking the `if` functionality, we don't need to
create a special function to handle this expression, and we can keep using
`_PyAST_If`.

Now you need to run:
```
make regen-all -j8
```

> [!NOTE]
> In the previous snippet, the `invalid_if_stmt` was not replaced, but it can
> be replaced. The decision is only to simplify the steps.

> [!OPTIONAL]
> Feeling brave? what about you repeat the process for more keywords like
`while`, `for`, `and`, etc?

