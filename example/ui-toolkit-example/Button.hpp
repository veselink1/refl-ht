#define REFL_METADATA_FILE "reflmd/Button.hpp"
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Element.hpp"

namespace ppui {

struct Border {
    int thickness;
    int color;
};

template <>
Border parse<Border>(std::string_view str)
{
    return Border{1, 0};
}

struct Button {

    REFL(ppui::UiProp())
    Border border;

    REFL(ppui::UiProp(ppui::UiPropType::Required))
    std::string content;

    static std::string parse_content(std::string_view content)
    {
        return content.data();
    }
};

} // namespace ppui

#include REFL_METADATA_FILE
PPUI_ELEMENT_REGISTER(ppui::Button);

#endif // BUTTON_HPP
