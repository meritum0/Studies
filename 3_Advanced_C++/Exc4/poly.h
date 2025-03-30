#ifndef POLY_H
#define POLY_H

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

template<typename T, std::size_t N = 0>
class poly;

namespace std {
    template<typename T, typename U, size_t N, size_t M>
    struct common_type<poly<T, N>, poly<U, M>> {
        using type = poly<common_type_t<T, U>, max(N, M)>;
    };

    template<typename T, typename U, size_t N>
    struct common_type<U, poly<T, N>> {
        using type = poly<common_type_t<T, U>, max(N, static_cast<size_t>(1))>;
    };

    template<typename T, typename U, size_t N>
    struct common_type<poly<T, N>, U> {
        using type = poly<common_type_t<T, U>, max(N, static_cast<size_t>(1))>;
    };
} // namespace std

namespace details {
    template<typename T>
    struct is_poly {
        static constexpr bool value = false;
    };

    template<typename T, std::size_t N>
    struct is_poly<poly<T, N>> {
        static constexpr bool value = true;
    };

    template<typename T>
    constexpr bool is_poly_v = is_poly<T>::value;
} // namespace details

template<typename... Args>
poly(Args...) -> poly<std::common_type_t<Args...>, sizeof...(Args)>;

template<typename T, std::size_t N>
class poly {
private:
    std::array<T, N> coefficients;

public:
    constexpr poly() : coefficients{} {
    }

    template<typename U, std::size_t M>
    requires (std::is_convertible_v<U, T> && M <= N)
    constexpr poly(const poly<U, M> &p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] = p[i];
        for (std::size_t i = M; i < N; ++i)
            coefficients[i] = {};
    }

    template<typename U, std::size_t M>
    requires(std::is_convertible_v<U, T> && M <= N)
    constexpr poly(poly<U, M> &&p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] = std::move(p[i]);
        for (std::size_t i = M; i < N; ++i)
            coefficients[i] = {};
    }

    template<typename U>
    requires(std::is_convertible_v<U, T> && !details::is_poly_v<std::decay_t<U>>)
    constexpr poly(U &&u) : coefficients{static_cast<T>(std::forward<U>(u))} {
    }

    template<typename... Args>
    requires (2 <= sizeof...(Args) && sizeof...(Args) <= N && (std::is_convertible_v<Args, T> && ...))
    constexpr poly(Args &&... args) : coefficients{static_cast<T>(std::forward<Args>(args))...} {
    }

    template<typename U, std::size_t M>
    requires(std::is_convertible_v<U, T> && M <= N)
    constexpr poly &operator=(const poly<U, M> &p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] = p[i];
        for (std::size_t i = M; i < N; ++i)
            coefficients[i] = {};
        return *this;
    }

    template<typename U, std::size_t M>
    requires(std::is_convertible_v<U, T> && M <= N)
    constexpr poly &operator=(poly<U, M> &&p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] = std::move(p[i]);
        for (std::size_t i = M; i < N; ++i)
            coefficients[i] = {};
        return *this;
    }

    constexpr T &operator[](std::size_t i) {
        return coefficients[i];
    }

    constexpr const T &operator[](std::size_t i) const {
        return coefficients[i];
    }

    constexpr std::size_t size() const {
        return N;
    }

    template<typename U, std::size_t M>
    requires(std::is_convertible_v<U, T> && M <= N)
    constexpr poly &operator+=(const poly<U, M> &p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] += p[i];
        return *this;
    }

    template<typename U, std::size_t M>
    requires(std::is_convertible_v<U, T> && M <= N)
    constexpr poly &operator-=(const poly<U, M> &p) {
        for (std::size_t i = 0; i < M; ++i)
            coefficients[i] -= p[i];
        return *this;
    }

    template<typename U>
    requires(std::is_convertible_v<U, T> && N != 0)
    constexpr poly &operator+=(const U &value) {
        coefficients[0] += value;
        return *this;
    }

    template<typename U>
    requires(std::is_convertible_v<U, T> && N != 0)
    constexpr poly &operator-=(const U &value) {
        coefficients[0] -= value;
        return *this;
    }

    template<typename U>
    requires(std::is_convertible_v<U, T>)
    constexpr poly &operator*=(const U &value) {
        for (std::size_t i = 0; i < N; ++i)
            coefficients[i] *= value;
        return *this;
    }

    template<typename U>
    requires(std::is_convertible_v<U, T>)
    constexpr poly &operator*=(const poly<U, 1> &p) {
        return (*this) *= p[0];
    }

    constexpr poly operator-() const {
        poly result;
        for (std::size_t i = 0; i < N; ++i)
            result[i] = -coefficients[i];
        return result;
    }

    template<typename U, std::size_t K>
    constexpr auto at(const std::array<U, K> &a) const {
        return array_at_impl(a, std::make_index_sequence<K>{});
    }

    constexpr auto at() const {
        return *this;
    }

    template<typename U, typename... Args>
    constexpr auto at(const U &first, const Args &... args) const {
        return compute_poly(first, args...);
    }

private:
    template<typename U, std::size_t deg = 0, typename... Args>
    constexpr auto compute_poly(const U &x, const Args &... args) const {
        if constexpr (N == 0)
            return T{};
        else if constexpr (deg == N - 1) {
            if constexpr (details::is_poly_v<T>)
                return coefficients[deg].at(args...);
            else
                return coefficients[deg];
        }
        else {
            if constexpr (details::is_poly_v<T>)
                return coefficients[deg].at(args...) + x * compute_poly<U, deg + 1>(x, args...);
            else
                return coefficients[deg] + x * compute_poly<U, deg + 1>(x, args...);
        }
    }

    template<typename U, std::size_t K, std::size_t... I>
    constexpr auto array_at_impl(const std::array<U, K> &a, std::index_sequence<I...>) const {
        return at(a[I]...);
    }
};

template<typename U, std::size_t M>
constexpr auto const_poly(const poly<U, M> &p) {
    poly<poly<U, M>, 1> q;
    q[0] = p;
    return q;
}

template<typename U, std::size_t M>
constexpr auto const_poly(poly<U, M> &&p) {
    poly<poly<U, M>, 1> q;
    q[0] = std::move(p);
    return q;
}

template<typename T, typename U, std::size_t N, std::size_t M>
constexpr auto operator+(const poly<T, N> &p, const poly<U, M> &q) {
    std::common_type_t<poly<T, N>, poly<U, M>> result = p;
    result += q;
    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator+(const poly<T, N> &p, const U &value) {
    std::common_type_t<poly<T, N>, U> result = p;
    result += value;
    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator+(const U &value, const poly<T, N> &p) {
    return p + value;
}

template<typename T, typename U, std::size_t N, std::size_t M>
constexpr auto operator-(const poly<T, N> &p, const poly<U, M> &q) {
    std::common_type_t<poly<T, N>, poly<U, M>> result = p;
    result -= q;
    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator-(const poly<T, N> &p, const U &value) {
    std::common_type_t<poly<T, N>, U> result = p;
    result -= value;
    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator-(const U &value, const poly<T, N> &p) {
    return -p + value;
}

template<typename T, typename U, std::size_t N, std::size_t M>
constexpr auto operator*(const poly<T, N> &p, const poly<U, M> &q) {
    constexpr std::size_t result_size = (N > 0 && M > 0) ? (N + M - 1) : 0;
    poly<decltype(T{} * U{}), result_size> result{};

    for (std::size_t i = 0; i < N; ++i)
        for (std::size_t j = 0; j < M; ++j)
            result[i + j] += p[i] * q[j];

    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator*(const poly<T, N> &p, const U &value) {
    std::common_type_t<poly<T, N>, U> result = p;
    result *= value;
    return result;
}

template<typename T, typename U, std::size_t N>
constexpr auto operator*(const U &value, const poly<T, N> &p) {
    return p * value;
}

template<typename T, typename U, std::size_t N, std::size_t M>
constexpr auto cross(const poly<T, N> &p, const poly<U, M> &q) {
    if constexpr (details::is_poly_v<T>) {
        poly<decltype(cross(T{}, poly<U, M>{})), N> result;
        for (std::size_t i = 0; i < N; ++i)
            result[i] = cross(p[i], q);
        return result;
    } else {
        poly<decltype(T{} * poly<U, M>{}), N> result;
        for (std::size_t i = 0; i < N; ++i)
            result[i] = p[i] * q;
        return result;
    }
}

#endif // POLY_H
