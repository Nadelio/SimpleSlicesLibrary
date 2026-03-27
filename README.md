# Simple String Slices Library

This is a simple C library that aims to provide quick, easy, and sane strings to C in the form of slices.

This library includes versions of all the functions within `string.h` that work with specifically slices, so that you don't have to deal with `string.h` at all.

If for some reason you want to convert slices back to C strings, there is also a function for that.

Go ham.

## Compiling from source
```bash
git clone https://github.com/Nadelio/SimpleSlicesLibrary.git
cd SimpleSlicesLibrary
mkdir build && cd build
cmake ..
cmake --build . --config Release
cmake --install . --config Release
```

Since the `CMakeLists.txt` file is built around using Clang, you may get warnings if you are using a different compiler. There shouldn't be any major issues though, as I've tested this with gcc and msvc and had no issues besides the warnings.