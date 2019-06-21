# RHT - Refl-cpp Header Tool (WIP)
## A C++ preprocessor that generated static reflection metadata for refl-cpp.

## How to use:
Just annotate the desired struct/class/member with the $refl() annotation and #include "filename.reflmd" at the end of the file.
If the target filename is User.hpp then append #include "User.reflmd" at the bottom. Reflmd files are C++ source files and do not require to be treated specially by the compiler. They contain the required REFL_TYPE/FUNC/FIELD macros, that would otherwise need to be manually written out.

## How to compile: 
g++ -std=c++17 -O2 -Wall -I/usr/lib/llvm-6.0/include rht-main.cpp -lstdc++fs -lclang