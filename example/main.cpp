#include "A.hpp"

int main()
{
    static_assert(refl::is_reflectable<A>());
}