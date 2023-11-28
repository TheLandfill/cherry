#pragma once
#include "polynomial.hpp"
#include "division-ring.hpp"
#include "identities.hpp"
#include "remove-v-tables.hpp"
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <string>

namespace cherry {

// Defined so that the product of any two numbers in the finite field
// fits in the data type.
#define MAX_TYPE(p) typename std::conditional< \
		p <= 16, \
		std::uint8_t, \
		typename std::conditional< \
			p <= UINT8_MAX, \
			std::uint16_t, \
			typename std::conditional< \
				p <= UINT16_MAX, \
				std::uint32_t, \
				std::uint64_t  \
			>::type \
		>::type \
	>::type

template<uint32_t p>
class GF1 COLON_IN_CLASS_INHERITANCE DIVISION_RING(GF1<p>) {
public:
	GF1(uint32_t v) : val(v % p) {}
	GF1() : val{0} {}

	GF1<p> operator+(const GF1<p>& other) const OVERRIDE {
		return (val + other.val) % p;
	}

	GF1<p>& operator+=(const GF1<p>& other) OVERRIDE {
		val += other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator-() const OVERRIDE {
		return p - val;
	}

	void negate() OVERRIDE {
		val = p - val;
	}

	GF1<p> operator-(const GF1<p>& other) const OVERRIDE {
		return 2 * p + val - other.val;
	}

	GF1<p>& operator-=(const GF1<p>& other) OVERRIDE {
		val += p - other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator*(const GF1<p>& other) const OVERRIDE {
		return val * other.val;
	}

	GF1<p>& operator*=(const GF1<p>& other) OVERRIDE {
		val *= other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator/(const GF1<p>& other) const OVERRIDE {
		return *this * (other^-1);
	}

	GF1<p>& operator/=(const GF1<p>& other) OVERRIDE {
		*this *= (other^-1);
		return *this;
	}

	GF1<p> inv() const OVERRIDE {
		return (*this)^-1;
	}

	GF1<p> operator^(int pow) const OVERRIDE {
		if (pow < 0 && val == 0) {
			throw std::domain_error(
				"Trying to take the multiplicative inverse of "
				"the additive identity (i.e. dividing by zero)."
			);
		}
		pow %= p - 1;
		if (pow < 0) {
			pow += p;
		}
		return pow_u(pow);
	}

	GF1<p> pow_u(uint32_t pow) const OVERRIDE {
		if (val <= 1) {
			return val;
		}
		pow %= p - 1;
		GF1<p> out = 1;
		GF1<p> val_pow = val;
		while (pow > 0) {
			if (pow & 1) {
				out *= val_pow;
			}
			val_pow *= val_pow;
			pow >>= 1;
		}
		return out;
	}

	uint64_t get_val() const {
		return val;
	}

	bool operator==(const GF1<p>& other) const {
		return val == other.val;
	}

	std::string to_string() const {
		return std::to_string(static_cast<uint64_t>(val));
	}

	explicit operator std::string() const {
		return to_string();
	}
public:
	static GF1<p> member_zero(const void * other_data = nullptr) {
		(void)other_data;
		return 0;
	}

	static GF1<p> member_one(const void * other_data = nullptr) {
		(void)other_data;
		return 1;
	}
private:
	MAX_TYPE(p) val;
};

template<uint32_t p>
std::ostream& operator<<(std::ostream& s, const GF1<p>& value) {
	s << value.get_val();
	return s;
}

class GF_Mat_Base;
template<uint32_t p>
class GF_Mat;

template<uint32_t p, typename Repr = GF_Mat<p>>
class GF COLON_IN_CLASS_INHERITANCE DIVISION_RING(GF<p COMMA Repr>) {
public:
	GF(const Polynomial<GF1<p>>& irreducible_poly) :
		order(
			irreducible_poly.degree()
		),
		val(
			irreducible_poly
		)
	{}
	GF(const Polynomial<GF1<p>>& irreducible_poly, const Polynomial<GF1<p>>& poly) :
		order(
			irreducible_poly.degree()
		),
		val(
			irreducible_poly,
			poly
		)
	{}
	GF(const Repr& v) : val(v) {}
	using GFs = GF<p, Repr>;

	GFs operator+(const GFs& other) const OVERRIDE {
		GFs out{ *this };
		out.val = val + other.val;
		return out;
	}

	GFs& operator+=(const GFs& other) OVERRIDE {
		val += other.val;
		return *this;
	}

	GFs operator-() const OVERRIDE {
		GFs out{ *this };
		out.val = -out.val;
		return out;
	}

	void negate() OVERRIDE {
		val.negate();
	}

	GFs operator-(const GFs& other) const OVERRIDE {
		GFs out{ *this };
		out.val = val - other.val;
		return out;
	}

	GFs& operator-=(const GFs& other) OVERRIDE {
		val -= other.val;
		return *this;
	}

	GFs operator*(const GFs& other) const OVERRIDE {
		GFs out{ *this };
		out.val = val * other.val;
		return out;
	}

	GFs& operator*=(const GFs& other) OVERRIDE {
		val *= other.val;
		return *this;
	}

	GFs operator*(const GF1<p>& other) const {
		val *= other;
		return *this;
	}

	GFs& operator*=(const GF1<p>& other) {
		val *= other;
		return *this;
	}
	GFs operator/(const GFs& other) const OVERRIDE {
		return (*this) * other.inv();
	}

	GFs& operator/=(const GFs& other) OVERRIDE {
		(*this) *= other.inv();
		return *this;
	}

	GFs operator^(int pow) const OVERRIDE {
		GFs out{ *this };
		out.val = out.val^pow;
		return out;
	}

	GFs pow_u(uint32_t pow) const OVERRIDE {
		GFs out{ *this };
		out.val = out.val.pow_u(pow);
		return out;
	}

	GFs inv() const OVERRIDE {
		GFs out{ *this };
		out.val = out.val.inv();
		return out;
	}

	const Repr& get_val() const {
		return val;
	}

	bool operator==(const GFs& other) const {
		return val == other.val;
	}

	uint32_t get_order() const {
		return order;
	}

	std::string to_string() const {
		return val.to_string();
	}

	explicit operator std::string() const {
		return to_string();
	}
public:
	static GFs member_zero(const void * other_data) {
		const Polynomial<GF1<p>>& irreducible_poly = *static_cast<const Polynomial<GF1<p>>*>(other_data);
		return GFs{irreducible_poly};
	}

	static GFs member_one(const void * other_data) {
		const Polynomial<GF1<p>>& irreducible_poly = *static_cast<const Polynomial<GF1<p>>*>(other_data);
		return GFs{irreducible_poly, one<Polynomial<GF1<p>>>()};
	}
private:
	uint32_t order;
	Repr val;
};

template<uint32_t p, typename Repr>
GF<p, Repr> operator*(const GF1<p>& scalar, const GF<p, Repr>& vector) {
	GF<p, Repr> out{ vector };
	out *= scalar;
	return out;
}

template<uint32_t p, typename Repr>
std::ostream& operator<<(std::ostream& s, const GF<p, Repr>& value) {
	s << value.get_val();
	return s;
}

}
