/**
 * Created by veselink1.
 * Released under the MIT license.
 */

#ifndef REFL_HT_CORE_HPP
#define REFL_HT_CORE_HPP
#include <clang-c/Index.h>
#include "rht-util.hpp"

namespace rht::core
{
enum MemberType
{
    Field,
    Function
};

struct MemberInfo
{
    MemberType Type;
    std::string Name;
    std::string Attributes;

    MemberInfo(MemberType type, const std::string &name, const std::string &attributes)
        : Type(type), Name(name), Attributes(attributes)
    {
    }
};

struct TypeInfo
{
    std::string Name;
    std::string Attributes;
    std::vector<MemberInfo> Members;

    TypeInfo(const std::string &name, const std::string &attributes)
        : Name(name), Attributes(attributes), Members{}
    {
    }
};

} // namespace rht::core

#endif // REFL_HT_CORE_HPP