# OpenGL Game Engine

WIP

# Building

To build this project you will need:

- `cmake 3.28-rc2` at the very least
- `python 3.11` or higher
- `glad` installed with python [src](https://github.com/Olgardos/glad)

Linux

- `ninja v1.11.1` or higher.
- `llvm 16` or `gcc 14` or higher

Windows

- `Visual Studio 17 2022` or higher

---

- Install the libraries needed, windows users will need to run this as administrator since the script creates symlinks `./kge.py install`
- build `./kge.py build` or build **and** run `./kge.py run`. the `run` command builds and runs the program, `build_no_run` only runs.
