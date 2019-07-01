#define REFL_METADATA_FILE "reflmd/A.hpp"
#include "refl.hpp"

namespace myns
{

struct A
{
    REFL() int x;
    REFL() int y;
};

} // namespace myns

#include REFL_METADATA_FILE
