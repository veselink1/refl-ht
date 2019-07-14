#include "Button.hpp"
#include "Parser.hpp"
#include <iostream>

constexpr auto view_template = R"!<xml>!(

    <ppui::Button border="0">
        Button 1
    </ppui::Button>
    
    <ppui::Button border="1">
        Button 2
    </ppui::Button>

)!<xml>!";

int main()
{
    std::cout << "\nUI Toolkit Example:\n";
    std::vector<std::any> elements = ppui::parse_view(view_template);
    for (auto&& element : elements) {
        if (ppui::Button* btn = std::any_cast<ppui::Button>(&element)) {
            refl::runtime::debug(std::cout, *btn);
            std::cout << std::endl;
        }
    }
}