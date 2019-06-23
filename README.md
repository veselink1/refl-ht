# RHT - Refl-cpp Header Tool (WIP)
## A C++ preprocessor that generated static reflection metadata for refl-cpp.

## How to use:
Just annotate the desired struct/class/member with the $refl() annotation and #include "filename.reflmd" at the end of the file.
If the target filename is User.hpp then append #include "User.reflmd" at the bottom. Reflmd files are C++ source files and do not require to be treated specially by the compiler. They contain the required REFL_TYPE/FUNC/FIELD macros, that would otherwise need to be manually written out.

Then run the following command to generate appropriate .reflmd files for discovered header files used by the input .cpp file.
./refl-ht -input=&lt;path-to-input-cpp-file&gt; -verbose=&lt;y|n&gt;

## How to compile: 
g++ -std=c++17 -O2 -Wall -I/usr/lib/llvm-7/include rht-main.cpp -lstdc++fs -lclang -o refl-ht
(Use appropriate path for your version of llvm libs. On Ubuntu, you might need to install libclang.)