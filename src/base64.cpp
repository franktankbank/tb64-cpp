#include <vector>

#include <base64.hpp>
#include <turbob64.h>

#include <gsl/span>

auto triple_encode(std::string& input) -> std::string
{
    tb64ini(0, 0);

    auto bytes = std::as_bytes(std::span {input});  // std::byte view, type-safe

    const size_t in_len = input.size();
    const size_t max_len1 = TB64ENCLEN(in_len);
    const size_t max_len2 = TB64ENCLEN(max_len1);
    const size_t max_len3 = TB64ENCLEN(max_len2);

    // Use std::vector for automatic memory management
    std::vector<unsigned char> buf1(max_len1 + 1);
    std::vector<unsigned char> buf2(max_len2 + 1);
    std::vector<unsigned char> buf3(max_len3 + 1);

    // Encode three times
    const size_t len1 =
        tb64enc(reinterpret_cast<const unsigned char*>(bytes.data()),
                in_len,
                buf1.data());
    const size_t len2 = tb64enc(buf1.data(), len1, buf2.data());
    const size_t len3 = tb64enc(buf2.data(), len2, buf3.data());

    // Return as std::string
    return {buf3.begin(), buf3.begin() + static_cast<std::ptrdiff_t>(len3)};
}
