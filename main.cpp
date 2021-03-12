// vim: set ft=cpp et ts=4 sw=4:

#include <array>
#include <iostream>

template <size_t N>
struct fixed_string {
    std::array<char, N> contents;

    constexpr fixed_string(char const (&str)[N]) {
        for (size_t i = 0; i < N; ++i) {
            contents[i] = str[i];
        }
    }
    constexpr fixed_string(fixed_string const&) = default;

    constexpr operator std::string_view() const {
        return {contents.data(), N};
    }

    friend std::ostream& operator<<(std::ostream& o, const fixed_string& s) {
        return (o << std::string_view{s});
    }
};

// TODO
//

template <auto v>
void print() {
    std::cout << v << '\n';
}

int main() {
    print<fixed_string("Hello World")>();
    print<42>();
}
