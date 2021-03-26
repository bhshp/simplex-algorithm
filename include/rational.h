#pragma once

#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <algorithm>
#include <limits>
#include <numeric>
#include <type_traits>
#include <vector>

// declaration

template <typename T>
struct rational;

// alias
using ll = int64_t;

template <typename T>
using row = std::vector<T, std::allocator<T>>;

template <typename T>
using col = row<T>;

template <typename T>
using matrix = row<row<T>>;

using number = rational<ll>;

// definition

template <typename T = ll>
struct rational {
    using value_type = T;

    friend std::ostream& operator<<(std::ostream& os, const rational& r) {
        if (r.den == 1) {
            return os << r.num;
        }
        return os << r.num << '/' << r.den;
    }

    friend std::istream& operator>>(std::istream& is, rational& r) {
        // only support integer input
        T n;
        is >> n;
        r = rational{n};
        return is;
    }

   public:
    static_assert(std::is_integral_v<T> && std::is_signed_v<T>);

    rational(T _num, T _den) : num(_num), den(_den) {
        bool neg = (num < 0) ^ (den < 0);
        num = std::abs(num);
        den = std::abs(den);
        T gcd = std::gcd(num, den);
        num /= gcd;
        den /= gcd;
        if (neg) {
            num = -num;
        }
    }

    rational() : rational(0, 1) {}

    rational(T _n) : rational(_n, 1) {}

    rational(const rational& r) : rational(r.num, r.den) {}

    rational(rational&& r) : rational(r.num, r.den) {}

    rational& operator=(const rational& other) {
        this->num = other.num;
        this->den = other.den;
        return *this;
    }

    rational& operator=(rational&& other) {
        this->num = other.num;
        this->den = other.den;
        return *this;
    }

    rational operator-() const {
        return rational{-num, den};
    }

    rational abs() const {
        return *this < 0 ? -(*this) : *this;
    }

    rational operator+(const rational& other) const {
        return rational{num * other.den + den * other.num,
                        den * other.den};
    }

    rational operator-(const rational& other) const {
        return rational{num * other.den - den * other.num,
                        den * other.den};
    }

    rational operator*(const rational& other) const {
        return rational{num * other.num, den * other.den};
    }

    rational operator/(const rational& other) const {
        return rational{num * other.den, den * other.num};
    }

    rational& operator+=(const rational& other) {
        *this = *this + other;
        return *this;
    }

    rational& operator-=(const rational& other) {
        *this = *this - other;
        return *this;
    }

    rational& operator*=(const rational& other) {
        *this = *this * other;
        return *this;
    }

    rational& operator/=(const rational& other) {
        *this = *this / other;
        return *this;
    }

    bool operator==(const rational& other) const {
        return num == other.num && den == other.den;
    }

    bool operator!=(const rational& other) const {
        return !(*this == other);
    }

    bool operator<(const rational& other) const {
        return num * other.den < other.num * den;
    }

    bool operator>(const rational& other) const {
        return num * other.den > other.num * den;
    }

    bool operator<=(const rational& other) const {
        return *this == other || *this < other;
    }

    bool operator>=(const rational& other) const {
        return *this == other || *this > other;
    }

   private:
    T num;
    T den;
};

template <typename T, typename S>
void print(const matrix<T>& A, const row<S>& b, std::ostream& os = std::cout) {
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            os << A[i][j] << '\t';
        }
        os << "\t\t" << b[i] << std::endl;
    }
}

template <typename T>
void print(const matrix<T>& A, std::ostream& os = std::cout) {
    for (size_t i = 0; i < A.size(); i++) {
        for (size_t j = 0; j < A[i].size(); j++) {
            os << A[i][j] << (j + 1 == A[i].size() ? '\n' : '\t');
        }
        os << std::flush;
    }
}

template <typename T>
void print(const row<T>& b, std::ostream& os = std::cout) {
    for (size_t i = 0; i < b.size(); i++) {
        os << b[i] << (i + 1 == b.size() ? '\n' : '\t');
    }
    os << std::flush;
}

#endif  // RATIONAL_H_
