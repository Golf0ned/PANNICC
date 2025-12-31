# PANNICC: Prototype And Non-Negligibly Instructive C Compiler

A pedagogical attempt at recreating a chunk of LLVM's infrastructure from scratch.

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

Due to the pedagogical nature of this project, pull requests by users other than me ([@golf0ned](https://github.com/Golf0ned/)) will most likely be closed.

With that being said, you are free to submit any issues or bug reports that you may encounter on GitHub.

If you still wish to submit changes anyways, there is a git hook to enforce formatting. You can install it with:

```
git config --local core.hooksPath .githooks/
```

You can also indirectly support this project by [sponsoring me](https://github.com/sponsors/Golf0ned).
