# refl-ht (Discontinued) - [refl-cpp](https://github.com/veselink1/refl-cpp) Header Tool (v0.4.0-beta) 
## A fast, portable and non-intrusive C++ preprocessor that generates static reflection metadata for [refl-cpp](https://github.com/veselink1/refl-cpp).

Minimum refl-cpp version required: v0.3.1

## How to use:
Just annotate the desired struct/class/member with the `REFL()` macro and do `#include REFL_METADATA_FILE` at the end of the file (will be appended automatically if omitted).
If the target filename is User.hpp then append `#include "User.reflmd"` at the bottom. Reflmd files are C++ source files and do not require to be treated specially by the compiler. They contain the required `REFL_TYPE/FUNC/FIELD` macros, that would otherwise need to be manually written out.

Then run the following command to generate appropriate .reflmd files for discovered header files used by the input .cpp file.
`./refl-ht -input=<filename> -verbose=<y|n>`

## How to compile: 
`g++ -std=c++17 -O2 -Wall -I/usr/lib/llvm-7/include rht-main.cpp -lclang -o refl-ht`

(Use appropriate path for your version of llvm libs. On Ubuntu, you might need to install libclang.)

## Screenshots
![](https://raw.githubusercontent.com/veselink1/refl-ht/master/example/ui-toolkit-example/output.png)

## Download
**Prebuilt binaries - coming coon**

## Example

```
struct UserModel
{
    REFL(property(read)) uint64_t id;
    REFL(property(/* default: read_write */) std::string first_name;
    REFL(property()) std::string last_name;
    REFL(property()) std::string email;
    REFL(property()) ns::datetime date_of_birth;

    REFL() std::shared_ptr<UserRepository> get_repo() const;
};
```

## Changelog

- v0.4.0 - refl-ht now no-longer depends on std::filesystem and does not require special linker flags.
- v0.3.1 - Fixed a bug where input files would get improperly formatted.
