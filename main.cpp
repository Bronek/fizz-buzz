// vim: set ft=cpp et ts=4 sw=4:

#include <cstdio>
#include <iostream>
#include <array>
#include <type_traits>
#include <limits>


template <size_t N>
struct fixed_string {
    // _data_ should be private
    std::array<char, N> _data_;

    constexpr fixed_string() {
        for (size_t i = 0; i < N; ++i) {
            _data_[i] = 0;
        }
    }
    constexpr fixed_string(char const (&str)[N]) {
        for (size_t i = 0; i < N; ++i) {
            _data_[i] = str[i];
        }
        // always apply nul terminator
        _data_[N - 1] = 0;
    }
    constexpr fixed_string(fixed_string const&) = default;

    template <size_t Ns>
    constexpr auto append(fixed_string<Ns> const& other) const -> fixed_string<N + Ns - 1>
    {
        auto res = fixed_string<N + Ns - 1>{};
        for (size_t i = 0; i < N - 1; ++i) {
            res._data_[i] = _data_[i];
        }
        for (size_t i = 0; i < Ns; ++i) {
            res._data_[i + N - 1]  = other._data_[i];
        }
        return res;
    }

    constexpr auto append(char c) const -> fixed_string<N + 1>
    {
        auto res = fixed_string<N + 1>{};
        for (size_t i = 0; i < N - 1; ++i) {
            res._data_[i] = _data_[i];
        }
        res._data_[N - 1] = c;
        return res;
    }

    constexpr auto prepend(char c) const -> fixed_string<N + 1>
    {
        auto res = fixed_string<N + 1>{};
        for (size_t i = 1; i <= N; ++i) {
            res._data_[i] = _data_[i - 1];
        }
        res._data_[0] = c;
        return res;
    }

    constexpr auto c_str() const -> char const* {
        return _data_.data();
    }

    constexpr operator std::string_view() const {
        return {_data_.data(), N};
    }

    friend auto operator<<(std::ostream& o, const fixed_string& s) -> std::ostream& {
        return (o << std::string_view{s});
    }
};

constexpr auto digits(unsigned int i) {
    static_assert(sizeof(decltype(i)) == 4);
    int res = 1;
    while (i > 9) {
        i /= 10;
        res += 1;
    }
    return res;
}

template <size_t N>
constexpr auto format() -> fixed_string<digits(N) + 1> {
    if constexpr (N > 9)
        return format<(N / 10)>().append('0' + (N % 10));
    else {
        const char buf[2] = {'0' + N, 0};
        return fixed_string{buf};
    }
}



int main() {
    constexpr auto fizz = fixed_string{""}.append(fixed_string{"fizz"});
    std::printf("%s\n", fizz.c_str());
    constexpr auto buzz = fixed_string{"buzz"};
    std::printf("%s\n", buzz.c_str());
    constexpr auto fizzbuzz = fizz.append(buzz);
    std::printf("%s\n", fizzbuzz.c_str());
    std::cout << fizzbuzz.append(']').prepend('[') << '\n';
    constexpr auto num = format<42>();
    std::puts(fixed_string{"num:"}.append(num).c_str());
}
