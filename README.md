# libCGeo

`libCGeo` is an open source, cross-platform, performant C library for Computational Geometry. It allows for simple
function calls for Computational Geometry functions with C and C++ programs. This library is currently in development,
and does not yet have any stable releases.

### Building libCGeo from source

libCGeo can be built from source using CMake. So far, I have only tested this on an Ubuntu 18.04_LTS machine. To build using CMake:
```
mkdir build
cd build 
cmake ..
```
and then on linux type `make` to compile the library. This will also compile some example programs.