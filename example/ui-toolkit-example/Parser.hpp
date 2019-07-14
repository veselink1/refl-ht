#ifndef PARSER_HPP
#define PARSER_HPP
#include <stdexcept>
#include <regex> 
#include <any>
#include "Element.hpp"

namespace ppui {

class ParsingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::string trim_xml_content(std::string xml)
{
    static const std::basic_regex<char> nonWsRegex("\\S+");

    std::string s;
    s.reserve(xml.size());

    std::smatch matches;
    while (std::regex_search(xml, matches, nonWsRegex)) {
        s.append(matches[0].str()).push_back(' ');
        xml = matches.suffix().str();
    }

    if (s.size() > 1) {
        size_t cap = s.capacity();
        s.resize(s.size() - 1);
        if (cap == s.capacity()) {
            s.shrink_to_fit();
        }
    }
    return s;
}

std::vector<std::any> parse_view(std::string_view viewTemplate)
{
    static const std::basic_regex<char> xmlRegex(R"raw(<([\w:]+)([^>]*)>([\s\S]*?)</\s*\1\s*>)raw");

    std::vector<std::any> elements;
    std::string view(viewTemplate);

    std::smatch matches;
    while (std::regex_search(view, matches, xmlRegex)) {
        if (matches.size() < 4) {
            throw std::runtime_error("Error in XML parser!");
        }

        auto&& tagName = matches[1];
        auto&& attributes = matches[2];
        auto&& content = matches[3];
        
        UiProps props = parse<UiProps>(attributes.str());
        props["content"] = trim_xml_content(content.str());

        UiElementMetadata metadata = UiElementRegistry::get().find(tagName.str());
        std::any element = metadata.create_instance(props);

        view = matches.suffix().str();

        elements.push_back(element);
    }

    return elements;
}

} // namesapce ppui

#endif // PARSER_HPP