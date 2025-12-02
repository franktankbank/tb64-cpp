#include "CLI/CLI.hpp"
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
#include <version.h>
#ifdef WITH_HOMEBREW
#    include <turbobase64/turbob64.h>
#else
#    include <turbob64.h>
#endif

using std::string;

auto main(int argc, char** argv) -> int
{
    CLI::App app {"A tool to encode text into base64 3 times", "TB64"};
    string input;
    app.add_option("input", input, "Input string");
    bool do_version = false;
    app.add_flag("-v,--version", do_version, "Print version and exit");
#if defined __unix__ || __APPLE__
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    int width = static_cast<int>(w.ws_col) - 4;
#endif
#ifdef _WIN32
    SetConsoleOutputCP(magic::codepage);
#endif
    CLI11_PARSE(app, argc, argv);
    if (input == "" and !do_version) {
        struct box box_params;
        box_params = (struct box) {.text1 = "Invalid Argument",
                                   .text_color_hex1 = "#DC143C",
                                   .text2 = "No argument provided",
                                   .text_color_hex2 = "#FFEA00",
                                   .sep_color_hex = "#FF7F50",
                                   .box_color_hex = "#7CFC00",
                                   .error = true};
        print_box(box_params);
        return 1;
    }
    if (do_version) {
        struct box box_params;
        box_params = (struct box) {.text1 = "Version",
                                   .text_color_hex1 = "#00CED1",
                                   .text2 = tb64_version,
                                   .text_color_hex2 = "#FFEA00",
                                   .sep_color_hex = "#FF69B4",
                                   .box_color_hex = "#7CFC00",
                                   .error = false};
        print_box(box_params);
        return 0;
    }
    string encoded = triple_encode(input);
#if defined __unix__ || __APPLE__
    size_t length = encoded.length() + magic::padding;
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
    clip::set_text(encoded);
    return 0;
}
