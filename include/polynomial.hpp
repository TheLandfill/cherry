#pragma once
#include "ring.hpp"
#include <iostream>
#include <vector>

class Polynomial_Base {};

template<typename R>
class Polynomial : public Polynomial_Base {
public:
	Polynomial(const std::vector<R>& c) : coeffs(c) {}

	Polynomial<R> operator-() const {
		std::vector<R> new_coeffs;
		new_coeffs.reserve(coeffs.size());
		for (const R& coeff : coeffs) {
			new_coeffs.push_back(-coeff);
		}
		return Polynomial<R>{new_coeffs};
	}

	Polynomial<R> operator+(Polynomial<R> other) const {
		size_t len = std::max(coeffs.size(), other.coeffs.size());
		size_t min_len = std::min(coeffs.size(), other.coeffs.size());
		std::vector<R> new_coeffs;
		new_coeffs.reserve(len);
		for (size_t i = 0; i < min_len; i++) {
			new_coeffs.push_back(coeffs[i] + other.coeffs[i]);
		}
		const std::vector<R> * coeffs_to_add = &coeffs;
		if (coeffs.size() < other.coeffs.size()) {
			coeffs_to_add = &other.coeffs;
		}
		new_coeffs.insert(
			new_coeffs.end(),
			coeffs_to_add->begin() + min_len,
			coeffs_to_add->end()
		);
		while (!new_coeffs.empty() && new_coeffs.back() == 0) {
			new_coeffs.pop_back();
		}
		return Polynomial<R>{new_coeffs};
	}

	Polynomial<R> operator-(Polynomial<R> other) const {
		return *this + (-other);
	}

	R& operator[](size_t i) {
		return coeffs.at(i);
	}

	R operator[](size_t i) const {
		return coeffs.at(i);
	}

	size_t degree() const {
		return coeffs.size() - 1;
	}

	template<typename T>
	T eval(const T& val, void * other_data = nullptr) const {
		T out = zero(&out, other_data);
		T identity = one(&out, other_data);
		for (size_t i = degree(); i <= degree(); i--) {
			out *= val;
			out += coeffs[i] * identity;
		}
		return out;
	}
public:
	const static R* literally_just_to_store_type_data() {
		static R val;
		return &val;
	}
private:
	std::vector<R> coeffs;
};

template<typename T, std::enable_if_t<std::is_base_of_v<Polynomial_Base, T>, bool> = true>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	return {{ zero(T::literally_just_to_store_type_data(), other_data) }};
}

template<typename T, std::enable_if_t<std::is_base_of_v<Polynomial_Base, T>, bool> = true>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	return {{ one(T::literally_just_to_store_type_data(), other_data) }};
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const Polynomial<T>& polynomial) {
	s << "(";
	for (size_t i = polynomial.degree(); i > 0; i--) {
		s << polynomial[i] << ", ";
	}
	s << polynomial[0] << ")";
	return s;
}
