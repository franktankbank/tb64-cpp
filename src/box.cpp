#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

#include <box.hpp>
#include <magic.hpp>

using std::cerr;
using std::cout;
using std::string;
using std::stringstream;

constexpr std::string_view reset = "\x1b[0m";
constexpr std::string_view newline = "\n";

namespace
{
auto hex_to_rgb(const string& hex_color) -> string
{
    int red = 0;
    int green = 0;
    int blue = 0;
    const string hex = (hex_color[0] == '#') ? hex_color.substr(1) : hex_color;
    if (hex.length() == magic::six) {
        const string red_hex = hex.substr(0, 2);
        const string green_hex = hex.substr(2, 2);
        const string blue_hex = hex.substr(4, 2);
        red = stoi(red_hex, nullptr, magic::sixteen);
        green = stoi(green_hex, nullptr, magic::sixteen);
        blue = stoi(blue_hex, nullptr, magic::sixteen);
        stringstream ansi_color;
        ansi_color << "\x1b[38;2;" << red << ";" << green << ";" << blue << "m";
        return ansi_color.str();
    }
    cerr << "Invalid hex color code" << newline;
    return "";
}
}  // namespace

// Function to print a Unicode colored box with text
void print_box(const struct box& box_params)
{
    const std::size_t box_width =
        box_params.text1.length() + box_params.text2.length() + magic::five;
    const string text_color1 = hex_to_rgb(box_params.text_color_hex1);
    const string text_color2 = hex_to_rgb(box_params.text_color_hex2);
    const string sep_color = hex_to_rgb(box_params.sep_color_hex);
    const string box_color = hex_to_rgb(box_params.box_color_hex);
    // Top border
    switch (box_params.error) {
        case 1:
            cerr << "  " << box_color << "\u250F";  // Top-left corner
            for (int i = 0; std::cmp_less(i, box_width); i++) {
                cout << "\u2501";
            }  // Horizontal line
            cerr << "\u2513" << reset << newline;  // Top-right corner

            // Middle line with text
            cerr << "  " << box_color << "\u2503" << reset
                 << " ";  // Left border
            cerr << text_color1 << box_params.text1
                 << reset;  // First part of text
            cerr << " " << sep_color << "\u2192" << reset << " ";  // Separator
            cerr << text_color2 << box_params.text2
                 << reset;  // Second part of text
            cerr << " " << box_color << "\u2503" << reset
                 << newline;  // Right border

            // Bottom border
            cerr << "  " << box_color << "\u2517";  // Bottom-left corner
            for (int i = 0; std::cmp_less(i, box_width); i++) {
                cout << "\u2501";
            }  // Horizontal line
            cerr << "\u251B" << reset << newline;  // Bottom-right corner
            break;
        case 0:
            cout << "  " << box_color << "\u250F";  // Top-left corner
            for (int i = 0; std::cmp_less(i, box_width); i++) {
                cout << "\u2501";
            }  // Horizontal line
            cout << "\u2513" << reset << newline;  // Top-right corner

            // Middle line with text
            cout << "  " << box_color << "\u2503" << reset
                 << " ";  // Left border
            cout << text_color1 << box_params.text1
                 << reset;  // First part of text
            cout << " " << sep_color << "\u2192" << reset << " ";  // Separator
            cout << text_color2 << box_params.text2
                 << reset;  // Second part of text
            cout << " " << box_color << "\u2503" << reset
                 << newline;  // Right border

            // Bottom border
            cout << "  " << box_color << "\u2517";  // Bottom-left corner
            for (int i = 0; std::cmp_less(i, box_width); i++) {
                cout << "\u2501";
            }  // Horizontal line
            cout << "\u251B" << reset << newline;  // Bottom-right corner
            break;
        default:
            cerr << "Invalid error value, use 1 or 0" << newline;
            break;
    }
}
