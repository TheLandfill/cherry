#pragma once
#include "identities.hpp"
#include <cstddef>
#include <cmath>
#include <array>
#include <iostream>

namespace cherry {

template<size_t N = 1, typename T = double>
class Vector {
public:
	T operator[](size_t i) const;
	T& operator[](size_t i);
	// inverse
	void invert();
	Vector<N, T> operator-() const;
	// vector addition
	Vector<N, T>  operator+ (const Vector<N, T>& v) const;
	Vector<N, T>& operator+=(const Vector<N, T>& v);
	// vector subtraction
	Vector<N, T>  operator- (const Vector<N, T>& v) const;
	Vector<N, T>& operator-=(const Vector<N, T>& v);
	// scalar multiplication
	Vector<N, T>  operator* (const T& s) const;
	Vector<N, T>& operator*=(const T& s);
	// dot product
	T    operator* (const Vector<N, T>& s) const;
	// magnitude
	T    sqr_magnitude() const;
	T    magnitude() const;
	// normalizes the vector
	void normalize();
	// returns a normalized copy of the vector
	Vector<N, T> normalized() const;
	Vector<N, T> get_part_of_vector_not_in_direction(const Vector<N, T>& v) const;
	void to_zero_vector();
public:
	//std::array<T, N> x;
	T x[N];
};

template<size_t N, typename T>
T Vector<N, T>::operator[](size_t i) const {
	return x[i];
}

template <size_t N, typename T>
T& Vector<N, T>::operator[](size_t i) {
	return x[i];
}

template <size_t N, typename T>
void Vector<N, T>::invert() {
	for (T& i : x) {
		i = -i;
	}
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator-() const {
	Vector<N, T> out = *this;
	out.invert();
	return out;
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator+(const Vector<N, T>& v) const {
	Vector<N, T> out;
	for (size_t i = 0; i < N; i++) {
		out[i] = x[i] + v[i];
	}
	return out;
}

template <size_t N, typename T>
Vector<N, T>& Vector<N, T>::operator+=(const Vector<N, T>& v) {
	for (size_t i = 0; i < N; i++) {
		x[i] += v[i];
	}
	return *this;
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator-(const Vector<N, T>& v) const {
	Vector<N, T> out;
	for (size_t i = 0; i < N; i++) {
		out[i] = x[i] - v[i];
	}
	return out;
}

template <size_t N, typename T>
Vector<N, T>& Vector<N, T>::operator-=(const Vector<N, T>& v) {
	for (size_t i = 0; i < N; i++) {
		x[i] -= v[i];
	}
	return *this;
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::operator*(const T& s) const {
	Vector<N, T> out;
	for (size_t i = 0; i < N; i++) {
		out[i] = x[i] * s;
	}
	return out;
}

template <size_t N, typename T>
Vector<N, T>& Vector<N, T>::operator*=(const T& s) {
	for (size_t i = 0; i < N; i++) {
		x[i] *= s;
	}
	return *this;
}

template <size_t N, typename T>
T Vector<N, T>::operator*(const Vector<N, T>& v) const {
	T out = zero<T>();
	for (size_t i = 0; i < N; i++) {
		out += x[i] * v[i];
	}
	return out;
}

template <size_t N, typename T>
T Vector<N, T>::sqr_magnitude() const {
	return (*this) * (*this);
}

template <size_t N, typename T>
T Vector<N, T>::magnitude() const {
	return sqrt(sqr_magnitude());
}

template <size_t N, typename T>
void Vector<N, T>::normalize() {
	(*this) *= inv<T>(magnitude());
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::normalized() const {
	return (*this) * inv<T>(magnitude());
}

template <size_t N, typename T>
Vector<N, T> Vector<N, T>::get_part_of_vector_not_in_direction(const Vector<N, T>& v) const {
	return *this - ((*this) * v) / (v * v) * v;
}

template <size_t N, typename T>
void Vector<N, T>::to_zero_vector() {
	std::fill(x, x + N, zero<T>());
}

template <size_t N, typename T>
Vector<N, T> operator*(T r, const Vector<N, T>& v) {
	return v * r;
}

template <size_t N, typename T>
T angle_between_vectors(const Vector<N, T>& v1, const Vector<N, T>& v2) {
	T numerator = v1 * v2;
	T denominator = sqrtf(v1.sqr_magnitude() * v2.sqr_magnitude());
	return std::acos(numerator / denominator);
}

}

template <size_t N, typename T>
std::ostream& operator<<(std::ostream& os, const cherry::Vector<N, T>& v)
{
	for (int i = 0; i < (int64_t)N - 1; i++) {
		os << v.x[i] << ", ";
	}
	os << v.x[N - 1];
    return os;
}
