#pragma once
#include "ring.hpp"
#include "identities.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

namespace cherry {

class Polynomial_Base {};

template<typename R>
class Polynomial : public Polynomial_Base {
public:
	Polynomial() {}
	Polynomial(const std::vector<R>& c) : coeffs(c) {}

	Polynomial<R> operator-() const {
		std::vector<R> new_coeffs;
		new_coeffs.reserve(coeffs.size());
		for (const R& coeff : coeffs) {
			new_coeffs.push_back(-coeff);
		}
		return Polynomial<R>{new_coeffs};
	}

	Polynomial<R> operator+(const Polynomial<R>& other) const {
		R z = zero<R>();
		size_t len = std::max(coeffs.size(), other.coeffs.size());
		std::vector<R> new_coeffs;
		new_coeffs.reserve(len);
		for (size_t i = 0; i < len; i++) {
			new_coeffs.push_back((*this)[i] + other[i]);
		}
		while (!new_coeffs.empty() && new_coeffs.back() == z) {
			new_coeffs.pop_back();
		}
		return Polynomial<R>{new_coeffs};
	}

	Polynomial<R> operator-(const Polynomial<R>& other) const {
		static const R z = zero<R>();
		size_t len = std::max(coeffs.size(), other.coeffs.size());
		std::vector<R> new_coeffs;
		new_coeffs.reserve(len);
		for (size_t i = 0; i < len; i++) {
			new_coeffs.push_back((*this)[i] - other[i]);
		}
		while (!new_coeffs.empty() && new_coeffs.back() == z) {
			new_coeffs.pop_back();
		}
		return Polynomial<R>{new_coeffs};
	}

	Polynomial<R> operator*(const Polynomial<R>& other) const {
		static const R z = zero<R>();
		std::vector<R> new_coeffs;
		new_coeffs.reserve(coeffs.size() + other.coeffs.size());
		for (size_t i = 0; i < coeffs.size() + other.coeffs.size(); i++) {
			new_coeffs.emplace_back(z);
		}
		for (size_t i = 0; i < coeffs.size(); i++) {
			const R& v_this = coeffs.at(i);
			for (size_t j = 0; j < other.coeffs.size(); j++) {
				const R& v_other = other.coeffs.at(j);
				new_coeffs.at(i + j) += v_this * v_other;
			}
		}
		while (!new_coeffs.empty() && new_coeffs.back() == z) {
			new_coeffs.pop_back();
		}
		return new_coeffs;
	}

	Polynomial<R>& operator*=(const R& other) {
		for (auto& coeff : coeffs) {
			coeff *= other;
		}
		return *this;
	}

	R& operator[](size_t i) {
		return coeffs.at(i);
	}

	R operator[](size_t i) const {
		if (i < coeffs.size()) {
			return coeffs[i];
		} else {
			return zero<R>();
		}
	}

	size_t degree() const {
		if (coeffs.size() == 0) {
			return 0;
		} else {
			return coeffs.size() - 1;
		}
	}

	// Horner's Rule
	template<typename T>
	T eval(const T& val, void * other_data = nullptr) const {
		T out = cherry::zero<T>(other_data);
		T identity = cherry::one<T>(other_data);
		for (size_t i = degree(); i <= degree(); i--) {
			out *= val;
			out += coeffs[i] * identity;
		}
		return out;
	}

	void to_derivative() {
		if (degree() == 0) {
			coeffs.clear();
		}
		for (size_t i = 1; i < coeffs.size(); i++) {
			coeffs[i - 1] = static_cast<R>(i) * coeffs[i];
		}
		coeffs.pop_back();
	}

	void to_derivative(size_t n) {
		if (n > degree()) {
			coeffs.clear();
		}
		for (size_t i = n; i < coeffs.size(); i++) {
			size_t multiple = 1;
			for (size_t j = i; j > i - n; j--) {
				multiple *= j;
			}
			coeffs[i - n] = static_cast<R>(multiple) * coeffs[i];
		}
		coeffs.resize(coeffs.size() - n);
	}

	Polynomial<R> differentiate() const {
		Polynomial<R> out;
		out.coeffs.reserve(coeffs.size());
		for (size_t i = 1; i < coeffs.size(); i++) {
			out.push_back(static_cast<R>(i) * coeffs[i]);
		}
		return out;
	}

	Polynomial<R> differentiate(size_t n) const {
		Polynomial<R> out{ *this };
		out.to_derivative(n);
		return out;
	}

	explicit operator std::string() const {
		return to_string();
	}

	std::string to_string() const {
		std::string out;
		out.reserve(64);
		out += "(";
		if (degree() != 0) {
			for (size_t i = degree(); i > 0; i--) {
				out += static_cast<std::string>((*this)[i]);
				out += ", ";
			}
		}
		out += static_cast<std::string>((*this)[0]);
		out += ")";
		return out;
	}
public:
	const static Polynomial<R> poly_zero(const void * other_data = nullptr) {
		(void)other_data;
		return {{ zero<R>(other_data) }};
	}

	const static Polynomial<R> poly_one(const void * other_data = nullptr) {
		return {{ one<R>(other_data) }};
	}
private:
	std::vector<R> coeffs;
};

template<typename T, std::enable_if_t<std::is_base_of_v<Polynomial_Base, T>, bool> = true>
constexpr T zero(const void * other_data = nullptr) {
	return T::poly_zero(other_data);
}

template<typename T, std::enable_if_t<std::is_base_of_v<Polynomial_Base, T>, bool> = true>
constexpr T one(const void * other_data = nullptr) {
	return T::poly_one(other_data);
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const Polynomial<T>& polynomial) {
	s << "(";
	if (polynomial.degree() != 0) {
		for (size_t i = polynomial.degree(); i > 0; i--) {
			s << polynomial[i] << ", ";
		}
	}
	s << polynomial[0] << ")";
	return s;
}

}
