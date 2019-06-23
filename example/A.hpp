#include "refl.hpp"

struct A
{
    $refl() int x;
    $refl() int y;
};

#include "A.reflmd"