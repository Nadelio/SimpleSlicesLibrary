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
cmake --build .
cmake --install .
```

Since the `CMakeLists.txt` file is built around using Clang, you may get warnings if you are using a different compiler. There shouldn't be any major issues though, as I've tested this with gcc and msvc and had no issues besides the warnings.

### Windows-specific Compilation Bullshit
Windows likes to force you to use MSVC if you have it downloaded at all on your computer, so use this instead of `cmake ..` to force it to behave:
```bash
cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_AR="C:/Program Files/LLVM/bin/llvm-ar.exe" -DCMAKE_RANLIB="C:/Program Files/LLVM/bin/llvm-ranlib.exe" -DCMAKE_BUILD_TYPE=Release ..
```
Also, not really super important for specifically my library, but if you want to force Clang to compile with purely the Clang toolchain and not a debug runtime (because of course MSVC shoves itself in as a debug runtime no matter what, causing an annoying linker error), use this compiler flag:
```bash
-fuse-ld=lld
```

### Compiliation Dependencies
- CMake
- Ninja
- Clang (recommended)