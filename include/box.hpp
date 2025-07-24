#pragma once
#ifndef BOX_HPP
#    define BOX_HPP
#    include <string>

struct box
{
    std::string text1;
    std::string text_color_hex1;
    std::string text2;
    std::string text_color_hex2;
    std::string sep_color_hex;
    std::string box_color_hex;
    bool error;
};

void print_box(const struct box& box_params);

#endif
