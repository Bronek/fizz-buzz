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

template <typename ...> struct data;

template <typename T, typename S, typename ... Ts>
struct data<T, S, Ts...> : data<Ts...> {
    T element;

    constexpr data(T&& v, data<Ts...>&& b)
        : data<Ts...>(std::move(b))
        , element(std::move(v))
    {}

    template <typename Sn>
    constexpr auto const& get() const {
        return this->select(std::type_identity_t<Sn>{});
    }

    template <typename Sn, typename Tn>
    constexpr auto append(Tn&& v) && {
        return data<Tn, Sn, T, S, Ts...>{std::move(v), std::move(*this)};
    }

protected:
    using data<Ts...>::select;
    constexpr auto const& select(std::type_identity_t<S>) const {
        return element;
    }
};

template <>
struct data<> {
    constexpr data() = default;

    template <typename Sn, typename Tn>
    constexpr auto append(Tn&& v) && {
        return data<Tn, Sn>{std::move(v), std::move(*this)};
    }

protected:
    constexpr void select() const {}
};

template <auto v>
void print() {
    std::cout << v << '\n';
}

enum HelloWorld{};
enum TheAnswer{};

int main() {
    constexpr auto d = data{}.
        append<HelloWorld>(fixed_string{"HelloWorld"}).
        append<TheAnswer>(42);
    print<d.get<HelloWorld>()>();
    print<d.get<TheAnswer>()>();
}
