#pragma once
#include "polynomial.hpp"
#include "division-ring.hpp"
#include "identities.hpp"
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace cherry {

// Defined so that the product of any two numbers in the finite field is zero.
#define MAX_TYPE(p) typename std::conditional< \
		p <= 16, \
		std::uint8_t, \
		typename std::conditional< \
			p <= UINT8_MAX, \
			std::uint16_t, \
			std::uint32_t \
		>::type \
	>::type

template<unsigned int p>
class GF1 : public Division_Ring<GF1<p>>, public GF1_Base {
public:
	GF1(unsigned int v) : val(v % p) {}
	GF1() : val{0} {}

	GF1<p> operator+(const GF1<p>& other) const override {
		return (val + other.val) % p;
	}

	GF1<p>& operator+=(const GF1<p>& other) override {
		val += other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator-() const override {
		return p - val;
	}

	void negate() override {
		val = p - val;
	}

	GF1<p> operator-(const GF1<p>& other) const override {
		return 2 * p + val - other.val;
	}

	GF1<p>& operator-=(const GF1<p>& other) override {
		val += p - other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator*(const GF1<p>& other) const override {
		return val * other.val;
	}

	GF1<p>& operator*=(const GF1<p>& other) override {
		val *= other.val;
		val %= p;
		return *this;
	}

	GF1<p> operator/(const GF1<p>& other) const override {
		return *this * (other^-1);
	}

	GF1<p>& operator/=(const GF1<p>& other) override {
		*this *= (other^-1);
		return *this;
	}

	GF1<p> inv() const override {
		return (*this)^-1;
	}

	GF1<p> operator^(int pow) const override {
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

	GF1<p> pow_u(unsigned int pow) const override {
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

	uint32_t get_val() const {
		return val;
	}

	bool operator==(const GF1<p>& other) const {
		return val == other.val;
	}

private:
	MAX_TYPE(p) val;
};

template<unsigned int p>
std::ostream& operator<<(std::ostream& s, const GF1<p>& value) {
	s << value.get_val();
	return s;
}

class GF_Mat_Base;
template<unsigned int p>
class GF_Mat;

template<unsigned int p, typename Repr = GF_Mat<p>>
class GF : public Division_Ring<GF<p, Repr>>, public GF_Base {
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

	GFs operator+(const GFs& other) const override {
		GFs out{ *this };
		out.val = val + other.val;
		return out;
	}

	GFs& operator+=(const GFs& other) override {
		val += other.val;
		return *this;
	}

	GFs operator-() const override {
		GFs out{ *this };
		out.val = -out.val;
		return out;
	}

	void negate() override {
		val.negate();
	}

	GFs operator-(const GFs& other) const override {
		GFs out{ *this };
		out.val = val - other.val;
		return out;
	}

	GFs& operator-=(const GFs& other) override {
		val -= other.val;
		return *this;
	}

	GFs operator*(const GFs& other) const override {
		GFs out{ *this };
		out.val = val * other.val;
		return out;
	}

	GFs& operator*=(const GFs& other) override {
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
	GFs operator/(const GFs& other) const override {
		return (*this) * other.inv();
	}

	GFs& operator/=(const GFs& other) override {
		(*this) *= other.inv();
		return *this;
	}

	GFs operator^(int pow) const override {
		GFs out{ *this };
		out.val = out.val^pow;
		return out;
	}

	GFs pow_u(unsigned int pow) const override {
		GFs out{ *this };
		out.val = out.val.pow_u(pow);
		return out;
	}

	GFs inv() const override {
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

	unsigned int get_order() const {
		return order;
	}
public:
	const static GFs gfs_zero(const void * other_data) {
		const Polynomial<GF1<p>>& irreducible_poly = *static_cast<const Polynomial<GF1<p>>*>(other_data);
		return GFs{irreducible_poly};
	}

	const static GFs gfs_one(const void * other_data) {
		const Polynomial<GF1<p>>& irreducible_poly = *static_cast<const Polynomial<GF1<p>>*>(other_data);
		return GFs{irreducible_poly, one<Polynomial<GF1<p>>>()};
	}
private:
	unsigned int order;
	Repr val;
};

template<unsigned int p, typename Repr>
GF<p, Repr> operator*(const GF1<p>& scalar, const GF<p, Repr>& vector) {
	GF<p, Repr> out{ vector };
	out *= scalar;
	return out;
}

template<unsigned int p, typename Repr>
std::ostream& operator<<(std::ostream& s, const GF<p, Repr>& value) {
	s << value.get_val();
	return s;
}

}
