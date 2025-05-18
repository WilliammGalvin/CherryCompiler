# 🍒 Cherry Compiler
Cherry is a programming language that I am developing. It's still in the EARLY EARLY stages but it
plans to be a quick automation language.
<br />
<br />


Cherry is a relatively fast language, it compiles source code into C. Currently, it's restricted
to one file ending in `.ch`. To compile your source code, launch the project with the path as the
first and only argument.
<br />

## 📖 Documentation
Right now, there isn't a lot, but watch as it grows!

### Data types
Currently, Cherry has no variables. However, here are some data types to use with functions.
<br />

- **String** - `"Hello, world!"`
- **Float** - `12.5`
- **Integer** - `7`

### Built-in functions
Cherry comes with some built-in functions that can be identifed by ending in `!` much like you'd see
with Rust macros. Currently, each function only can take one parameter.
<br />

`print! [value]`<br />
Print out a value. Supports all variable types.
<br />

`println! [value]`<br />
Print out a value and ends off the line. Supports all variable types.
