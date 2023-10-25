# OpenGL Game Engine

WIP

# Building

To build this project you will need `cmake 3.28-rc2` at the very least

## Linux

for linux besides `cmake 3.28` you will need `ninja v1.11.1` or higher.

CMake for linux expects the following

```
.
└── /usr/local/
    ├── include/
    │   ├── GLAD/
    │   │   └── glad.h
    │   ├── GLFW/
    │   │   └── glfw3.h
    │   ├── GLM/
    │   │   ├── glm.hpp
    │   │   └── other glm files...
    │   └── STB/
    │       └── stb_image.h
    └── lib/
        ├── libglad.a
        └── libglfw3.a
```

you can copy the exact structure or change `CMakeLists.txt` to build with another custom structure.
Although I plan to write a script to facilitate this task.

the script should take care of everything, run `./Build -r` to run the app, `./Build -b` to build, `./Build -h` for help.

## Windows

CMake for windows expects the following structure

```
.
└── libs/
    ├── include/
    │   ├── GLAD/
    │   │   └── glad.h
    │   ├── GLFW/
    │   │   └── glfw3.h
    │   ├── GLM/
    │   │   ├── glm.hpp
    │   │   └── other glm files...
    │   └── STB/
    │       └── stb_image.h
    ├── libglfw3.lib
    └── libglad.lib
```

the easiest way to do so is to extract the `bin/libs.zip` folder inside `./libs`, it contains the
built libraries aswell as the correct includes, although building manually is still encouraged.