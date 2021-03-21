// vim: set ft=cpp et ts=4 sw=4:

#include <cstdio>
#include <array>
#include <type_traits>
#include <string_view>

template <size_t N>
struct fixed_string {
private:
    template <size_t> friend struct fixed_string;

    struct tag_init{};
    constexpr static auto init() noexcept -> fixed_string {
        return fixed_string{tag_init{}};
    }

    constexpr fixed_string(tag_init) noexcept {
        _data_[N] = 0;
    }

    std::array<char, N + 1> _data_;

public:
    constexpr fixed_string() noexcept {
        static_assert(N == 0);
        _data_[0] = 0; // terminating nul
    }
    constexpr fixed_string(char const (&str)[N + 1]) noexcept  {
        for (size_t i = 0; i < N; ++i) {
            _data_[i] = str[i];
        }
        _data_[N] = 0; // terminating nul
    }
    constexpr fixed_string(fixed_string const&) noexcept = default;

    template <size_t Ns>
    constexpr auto append(fixed_string<Ns> const& other) const noexcept -> fixed_string<N + Ns> {
        auto res = fixed_string<N + Ns>::init();
        for (size_t i = 0; i < N; ++i) {
            res._data_[i] = _data_[i];
        }
        for (size_t i = 0; i < Ns; ++i) {
            res._data_[i + N]  = other._data_[i];
        }
        res._data_[N + Ns] = 0; // terminating nul
        return res;
    }

    constexpr auto append(char c) const noexcept -> fixed_string<N + 1> {
        auto res = fixed_string<N + 1>::init();
        for (size_t i = 0; i < N; ++i) {
            res._data_[i] = _data_[i];
        }
        res._data_[N] = c;
        res._data_[N + 1] = 0; // terminating nul
        return res;
    }

    constexpr auto prepend(char c) const noexcept -> fixed_string<N + 1> {
        auto res = fixed_string<N + 1>::init();
        res._data_[0] = c;
        for (size_t i = 1; i <= N; ++i) {
            res._data_[i] = _data_[i - 1];
        }
        res._data_[N + 1] = 0; // terminating nul
        return res;
    }

	// note: users of these two funtions should be able to deal with nul in the middle
    constexpr static size_t size() noexcept { return N; }
    constexpr operator std::string_view() const noexcept {
        return {_data_.data(), N};
    }

    // note: users of these two functions may not expect nul in the middle
    constexpr auto empty() const noexcept { return _data_[0] == 0; }
    constexpr auto c_str() const noexcept -> char const* { return _data_.data(); }
};

// CTAD for empty string
fixed_string() -> fixed_string<0>;

// CTAD for string literals
template <size_t N>
fixed_string(char const (&str)[N]) -> fixed_string<N - 1>;

constexpr auto digits(unsigned int i) noexcept {
    static_assert(sizeof(decltype(i)) == 4);
    int res = 1;
    while (i > 9) {
        i /= 10;
        res += 1;
    }
    return res;
}

template <unsigned int N>
constexpr auto format() noexcept -> fixed_string<digits(N)> {
    if constexpr (N > 9)
        return format<(N / 10)>().append('0' + (N % 10));
    else {
        const char buf[2] = {'0' + N, 0};
        return fixed_string{buf};
    }
}

template <unsigned int N>
constexpr auto fizzbuzz() noexcept {
    constexpr auto fizzpart = []<unsigned int Ns>() {
        if constexpr (Ns % 3 == 0) return fixed_string{"fizz"};
        else return fixed_string{};
    };
    constexpr auto buzzpart = []<unsigned int Ns>() {
        if constexpr (Ns % 5 == 0) return fixed_string{"buzz"};
        else return fixed_string{};
    };

    constexpr auto res = fixed_string{}
        .append(fizzpart.template operator()<N>())
        .append(buzzpart.template operator()<N>());

    if constexpr (res.empty()) return format<N>();
    else return res;
}

template <unsigned int ... N, typename Fn>
constexpr void fizzbuzz(std::integer_sequence<unsigned int, N...>, const Fn& fn) noexcept {
    (fn(fizzbuzz<N>()), ...);
}

template <unsigned int ... N>
constexpr auto plus1(std::integer_sequence<unsigned int, N...>) noexcept {
    return std::integer_sequence<unsigned int, (N + 1) ...>{};
}

int main() {
    fizzbuzz(plus1(std::make_integer_sequence<unsigned int, 100>{}), [](auto d){
        std::puts(d.c_str());
    });
}
