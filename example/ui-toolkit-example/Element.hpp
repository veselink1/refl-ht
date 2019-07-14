#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include "refl.hpp"
#include <any>
#include <sstream>
#include <functional>
#include <regex>

namespace ppui {

struct UiProps : public std::unordered_map<std::string, std::string> {
    using std::unordered_map<std::string, std::string>::unordered_map;
};

using UiElementCreator = std::any (*)(const UiProps&);

enum class UiPropType {
    Default = 0b0,
    Content = 0b1,
    Required = 0b10,
};

struct UiProp : refl::attr::usage::field {
    const UiPropType type;

    constexpr UiProp()
        : type(UiPropType::Default)
    {
    }

    constexpr UiProp(UiPropType type)
        : type(type)
    {
    }
};

namespace detail {
    template <typename T>
    T read_as(std::string_view str)
    {
        std::stringstream ss;
        ss << str;
        T val{};
        ss >> val;
        return val;
    }
}

template <typename T>
T parse(std::string_view str)
{
    static_assert(std::is_same_v<T, void>, "Missing specialization of ppui::parse<> for type T!");
    throw;
}

template <>
std::string parse<std::string>(std::string_view str)
{
    return str.data();
}

#define UI_DEFINE_PARSE_WITH_READAS(TypeName) \
    template <> \
    TypeName parse<TypeName>(std::string_view str) \
    { \
        return detail::read_as<TypeName>(str); \
    }

UI_DEFINE_PARSE_WITH_READAS(char);
UI_DEFINE_PARSE_WITH_READAS(signed char);
UI_DEFINE_PARSE_WITH_READAS(unsigned char);
UI_DEFINE_PARSE_WITH_READAS(signed short);
UI_DEFINE_PARSE_WITH_READAS(unsigned short);
UI_DEFINE_PARSE_WITH_READAS(signed int);
UI_DEFINE_PARSE_WITH_READAS(unsigned int);
UI_DEFINE_PARSE_WITH_READAS(signed long);
UI_DEFINE_PARSE_WITH_READAS(unsigned long);
UI_DEFINE_PARSE_WITH_READAS(signed long long);
UI_DEFINE_PARSE_WITH_READAS(unsigned long long);
UI_DEFINE_PARSE_WITH_READAS(float);
UI_DEFINE_PARSE_WITH_READAS(double);
UI_DEFINE_PARSE_WITH_READAS(long double);

#undef UI_DEFINE_PARSE_WITH_READAS

template <>
UiProps parse<UiProps>(std::string_view str)
{
    static const std::basic_regex<char> propRegex(R"raw(\s*(\w+)="([\s\S]*)")raw");
    UiProps props;

    std::string input(str);
    std::smatch matches;
    while (std::regex_search(input, matches, propRegex)) {
        auto&& propName = matches[1];
        auto&& propValue = matches[2];
        props.insert({ propName.str(), propValue.str() });
        input = matches.suffix().str();
    }

    return props;
}

class UiElementMetadata {
public:
    template <typename T>
    static UiElementMetadata create_metadata()
    {
        constexpr auto type = refl::reflect<T>();
        UiElementMetadata md;
        md.name_ = type.name;
        md.creator_ = &UiElementMetadata::create_untyped<T>;
        return md;
    }

    std::any create_instance(const UiProps& props) const
    {
        return creator_(props);
    }

    std::string_view name() const { return name_; }

private:
    std::string name_;
    UiElementCreator creator_;

    template <typename T>
    static std::any create_untyped(const UiProps& props)
    {
        T instance{};
        for_each(refl::reflect<T>().members, [&](auto member) {
            if constexpr (refl::descriptor::has_attribute<UiProp>(member)) {
                if (auto propIter = props.find(member.name.str());
                    propIter != props.end()) {
                    member(instance) = ppui::parse<typename decltype(member)::value_type>(propIter->second);
                }
            }
        });

        return std::move(instance);
    }
};

class UiElementRegistry {
public:
    static UiElementRegistry& get()
    {
        static UiElementRegistry instance;
        return instance;
    }

    UiElementMetadata find(std::string_view elementName) const
    {
        auto iter = std::find_if(metadata.begin(), metadata.end(), [&](auto&& x) {
            return x.name() == elementName;
        });
        if (iter == metadata.end()) {
            throw std::runtime_error("Element not found");
        }
        return *iter;
    }

    template <typename T>
    void register_type()
    {
        metadata.push_back(UiElementMetadata::create_metadata<T>());
    }

private:
    std::vector<UiElementMetadata> metadata;

    UiElementRegistry(){};
};

#define PPUI_CONCAT_(A, B) A##B
#define PPUI_CONCAT(A, B) PPUI_CONCAT_(A, B)

#define PPUI_STATIC_EXEC(Expression) \
    static int PPUI_CONCAT(_static_exec_dummy_, __COUNTER__) = (void(Expression), 0);
    
#define PPUI_ELEMENT_REGISTER(...) \
    PPUI_STATIC_EXEC(::ppui::UiElementRegistry::get().register_type<__VA_ARGS__>())

} // namespace ppui

#endif // UIELEMENT_HPP