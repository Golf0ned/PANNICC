# PANNICC: Prototype And Non-Negligibly Instructive C Compiler

A pedagogical attempt at recreating a chunk of LLVM's infrastructure from scratch.

## Language Support

Currently PANNICC supports the following subset of C:
- Variables and variable declarations
- Operators
    - Assignment (=, +=, -=, *=, /=, &=, |=, ^=)
    - Arithmetic operators (+, -, *, /)
    - Shift operators (<<, >>, <<=, >>=)
    - Some bitwise operators (&, |, ^)
    - Some unary operators (+, -, ~)
- Functions and scopes
- Some control flow (if, else, while)
- Return statements
- Comments

Refer to the grammar in [parser.cpp](https://github.com/Golf0ned/PANNICC/blob/main/src/frontend/parser.cpp) for more specifics.

## Installation/Usage

Clone the repo:

```
git clone git@github.com:Golf0ned/PANNICC.git
cd PANNICC
```

To build and run tests, run:

```
make
```

To view the help message, run:

```
./build/bin/pannicc
```

## Contributing

The best way to support this project is by [sponsoring me](https://github.com/sponsors/Golf0ned).

You are free to submit any issues or bug reports that you may encounter on GitHub. You are also free to submit new tests in the [Discord](https://discord.gg/KV2zChaX2J).

Due to the pedagogical nature of this project, pull requests by users other than me ([@golf0ned](https://github.com/Golf0ned/)) that implement/fix compiler functionality will likely be closed.

If you still wish to submit changes anyways, there is a git hook to enforce formatting. You can install it with:

```
git config --local core.hooksPath .githooks/
```

## Star History

<a href="https://www.star-history.com/#golf0ned/pannicc&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=golf0ned/pannicc&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=golf0ned/pannicc&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=golf0ned/pannicc&type=date&legend=top-left" />
 </picture>
</a>
