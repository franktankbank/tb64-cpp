#include <string>

#include <gsl/span>
#if defined __unix__ || __APPLE__
#    include <sys/ioctl.h>
#    include <unistd.h>
#endif
#ifdef _WIN32
#    include <windows.h>
#endif
#include <base64.hpp>
#include <box.hpp>
#include <clip.h>
#include <magic.hpp>

using std::string;

namespace
{
auto triple_encode(const string& data) -> string
{
    string pass1 = base64::to_base64(data);
    string pass2 = base64::to_base64(pass1);
    string pass3 = base64::to_base64(pass2);
    return pass3;
}
}  // namespace

auto main(int argc, char* argv[]) -> int
{
#if defined __unix__ || __APPLE__
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    int width = (int)(w.ws_col) - 4;
#endif
#ifdef _WIN32
    SetConsoleOutputCP(magic::codepage);
#endif
    if (argc != 2) {
        struct box box_params;
        box_params = (struct box) {
            .text1 = "Invalid Argument",
            .text_color_hex1 = "#DC143C",
            .text2 = "Please provide exactly one string as an argument",
            .text_color_hex2 = "#FFEA00",
            .sep_color_hex = "#FF7F50",
            .box_color_hex = "#7CFC00",
            .error = true};
        print_box(box_params);
        return 1;
    }
    const gsl::span<char*> args(argv, argc);
    string data = args[1];
    string encoded = triple_encode(data);
#if defined __unix__ || __APPLE__
    int length = encoded.length() + 7 + 5;
    if (length > width) {
        struct box box_params;
        box_params = (struct box) {.text1 = "Output Size Exceeded",
                                   .text_color_hex1 = "#DC143C",
                                   .text2 = "Output too large",
                                   .text_color_hex2 = "#FFEA00",
                                   .sep_color_hex = "#FF7F50",
                                   .box_color_hex = "#7CFC00",
                                   .error = true};
        print_box(box_params);
        return 1;
    }
#elif _WIN32
    if (encoded.length() > magic::max_width) {
        struct box box_params;
        box_params = (struct box) {.text1 = "Output Size Exceeded",
                                   .text_color_hex1 = "#DC143C",
                                   .text2 = "Output too large",
                                   .text_color_hex2 = "#FFEA00",
                                   .sep_color_hex = "#FF7F50",
                                   .box_color_hex = "#7CFC00",
                                   .error = true};
        print_box(box_params);
        return 1;
    }
#endif
    struct box box_params;
    box_params = (struct box) {.text1 = "Encoded",
                               .text_color_hex1 = "#00CED1",
                               .text2 = encoded,
                               .text_color_hex2 = "#FFEA00",
                               .sep_color_hex = "#FF69B4",
                               .box_color_hex = "#7CFC00",
                               .error = false};
    print_box(box_params);
    const string text = encoded;
    clip::set_text(text);
    return 0;
}
