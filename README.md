# RHT - Refl-cpp Header Tool (WIP)
## A C++ preprocessor that generated static reflection metadata for refl-cpp.

Minimum refl-cpp version required: v0.3.0

## How to use:
Just annotate the desired struct/class/member with the REFL() macro and add #include REFL_METADATA_FILE at the end of the file (will be appended automatically if omitted). Reflmd files are C++ source files and do not require to be treated specially by the compiler. They contain the required REFL_TYPE/FUNC/FIELD macros, that would otherwise need to be manually written out.

Then run the following command to generate appropriate .reflmd files for discovered header files used by the input .cpp file.
./refl-ht -input=&lt;path-to-input-cpp-file&gt; -verbose=&lt;yes|no&gt;

## How to compile: 
On Linux-based OSes:
`g++ -std=c++17 -O2 -Wall -I/usr/lib/llvm-7/include rht-main.cpp -lstdc++fs -lclang -o refl-ht`
On Windows 10 (using WSL):
(Build with the above command then type)
`echo bash -c './refl-ht %*' >> refl-ht.bat`
(Or write ./refl-ht %* to a .bat file manually)

(Use appropriate path for your version of llvm libs. On Ubuntu, you might need to install libclang.)

## Example:

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
