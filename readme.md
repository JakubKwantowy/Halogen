# Halogen #

Halogen is FORTH-like programming language.  
(this is a WIP, I might improve this some time later)

## Prerequisites ##

To build Halogen, you need the following utilities:

- GCC (or a similar C compiler)
- GNU Makefile

## Building ##

To build Halogen, run the following commands:

```bash
mkdir bin
make
```

Note: On some systems (such as BSD), replace `make` with `gmake`

## Tokens ##

Halogen scripts go through a process called `tokenization`, where the script gets broken down into individual tokens. Each token is split up by a space character. There exist multiple types of tokens.

- Words
- Numbers
- Word redefinition (`:`)
- Word redefinition terminator (`;`)
- Conditional loop (`(`)
- Conditional loop terminator (`)`)
- Comment (`#`)

### Words ###

Words are commands issued to the interpreter. Their behaviour is defined by the dictionary and can be (re)defined using a word redefinition.

Example: `print`, `swap`

### Number ###

A number that gets pushed onto the stack. Can either be decimal or hexadecimal (prefixed with 0x).

Example: `10`, `0x3f`

### Word redefinition ###

A colon followed by the word name. Adds / Changes the specified word in the dictionary. The word redefinition is closed via a redefinition terminator (`;`).

Example:

```halogen
:newword
    # Code here #
;
```

### Conditional loop ###

Loops between `(` and `)` while the value at the top of the stack is not 0. Note that the value DOES NOT get popped off the stack.

Example:

```halogen
5
(
    # For loop #
    print
    1 swap sub
)
```

### Comment ###

Makes interpreter ignore all tokens until the next comment token

Example:

```halogen
# Halogen is FORTH-like programming language. #
```
