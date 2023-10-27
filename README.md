# OpenGL Game Engine

WIP

# Building

To build this project you will need:

- `cmake 3.28-rc2` at the very least
- `python 3.11` or higher

Linux

- `ninja v1.11.1` or higher.
- `llvm 16` or `gcc 14` or higher
- `glad` pip package

Windows

- `Visual Studio 17 2022` or higher
- `requests` pip package

Steps:

- Install the libraries needed, windows users will need to run this as administrator since the script creates symlinks, run `python kge.py install`

---

- **FOR WINDOWS USERS ONLY** currently the glad2 python package appears to not work on windows, therefore you need to follow instructions

	**If you want to build everthing from scratch:**

	- open [this link](https://glad.dav1d.de/) and select the following
		- Language: C/C++
		- Specification: OpenGL
		- Api: gl 4.6 and the rest None
		- Profile: Core 
		- Extensions: Add All
		- Generate a loader: on, the rest off
	- Click *Generate*
	- Click on `glad.zip` and download that folder in `./libs/glad`
	- Extract the contents of the folder so that `include` `src` and `CMakeLists.txt` are in the same folder

	Repeat the following steps for `./libs/glfw` and `./libs/glad`

	- run `cmake -B build -G "Visual Studio 17 2022"`
	- run `cmake --build build --config release`

	For glfw you'll find `glfw3.lib` inside `./libs/glfw/build/src/Release`

	For glad you'll find `glad.lib` inside `./libs/glad/build/Release`

	move these inside `./libs/binaries`.

	**alternatively**

	[download](https://files.catbox.moe/twf44g.zip) the prebuilt binaries and put them inside `./libs/binaries`

---

- build `python kge.py build` or build **and** run `python kge.py run`. the `run` command builds and runs the program, `build_no_run` only runs.

- Use the binary inside `./build/Debug/kge.lib` or `./build/libkge.a`
