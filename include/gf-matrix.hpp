#pragma once
#include "finite-field.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <memory>
#include <stdexcept>

namespace cherry {

class GF_Mat_Base {};

template<unsigned int p>
class GF_Mat : DIVISION_RING(GF_Mat<p>) COMMA_IN_CLASS_INHERITANCE public GF_Mat_Base {
public:
	GF_Mat() {}
	GF_Mat(const Polynomial<GF1<p>>& irr) :
	irreducible_poly_ptr{std::make_shared<Polynomial<GF1<p>>>(irr)},
	val{
		GF1<p>{0},
		irr.degree(),
		irreducible_poly_ptr
	},
	lambda{
		GF1<p>{0},
		irr.degree(),
		irreducible_poly_ptr
	}
{
		lambda.to_nilpotent_diag(1);
		auto irreducible = irr;
		irreducible *= cherry::inv(irreducible[irreducible.degree()]);
		for (size_t i = 0; i < irreducible.degree(); i++) {
			lambda(irreducible.degree() - i - 1, 0) = -irreducible[i];
		}
	}

	GF_Mat(const Polynomial<GF1<p>>& irreducible, const Polynomial<GF1<p>>& vals) : val{GF1<p>{0}, irreducible.degree()}, lambda{GF1<p>{0}, irreducible.degree()} {
		lambda.to_nilpotent_diag(1);
		for (size_t i = 0; i < irreducible.degree(); i++) {
			lambda(irreducible.degree() - i - 1, 0) = -irreducible[i];
		}
		Matrix<GF1<p>> identity(GF1<p>(0), val.get_num_rows());
		identity.to_identity();
		for (size_t i = vals.degree(); i <= vals.degree(); i--) {
			val *= lambda;
			val += identity * vals[i];
		}
	}

	GF_Mat operator+(const GF_Mat& other) const OVERRIDE {
		check_if_same_polynomial(other);
		GF_Mat out{ *this };
		out.val += other.val;
		return out;
	}

	GF_Mat& operator+=(const GF_Mat& other) OVERRIDE {
		check_if_same_polynomial(other);
		val += other.val;
		return *this;
	}

	GF_Mat operator-() const OVERRIDE {
		GF_Mat out{ *this };
		out.val = -out.val;
		return out;
	}

	GF_Mat operator-(const GF_Mat& other) const OVERRIDE {
		check_if_same_polynomial(other);
		GF_Mat out{ *this };
		out.val -= other.val;
		return out;
	}

	void negate() OVERRIDE {
		val.negate();
	}

	GF_Mat& operator-=(const GF_Mat& other) OVERRIDE {
		check_if_same_polynomial(other);
		val -= other.val;
		return *this;
	}

	GF_Mat operator*(const GF_Mat& other) const OVERRIDE {
		check_if_same_polynomial(other);
		GF_Mat out{ *this };
		out.val *= other.val;
		return out;
	}

	GF_Mat operator*(const GF1<p>& other) const {
		GF_Mat out{ *this };
		out.val *= other.val;
		return out;
	}

	GF_Mat& operator*=(const GF_Mat& other) OVERRIDE {
		check_if_same_polynomial(other);
		val *= other.val;
		return *this;
	}

	GF_Mat& operator*=(const GF1<p>& other) {
		val *= other;
		return *this;
	}

	GF_Mat operator/(const GF_Mat& other) const OVERRIDE {
		check_if_same_polynomial(other);
		return (*this) * other.inv();
	}

	GF_Mat& operator/=(const GF_Mat& other) OVERRIDE {
		check_if_same_polynomial(other);
		(*this) *= other.inv();
		return *this;
	}

	GF_Mat operator^(int pow) const OVERRIDE {
		GF_Mat out{*this};
		out.val = out.val^pow;
		return out;
	}

	GF_Mat pow_u(unsigned int pow) const OVERRIDE {
		GF_Mat out{*this};
		out.val = out.val.pow_u(pow);
		return out;
	}

	GF_Mat inv() const OVERRIDE {
		GF_Mat out{*this};
		out.val = out.val.inv();
		return out;
	}

	void check_if_same_polynomial(const GF_Mat& other) const {
		if (lambda == other.lambda) {
			return;
		} else {
			throw std::domain_error("These two matrices come from different irreducible polynomials");
		}
	}

	const Matrix<GF1<p>>& get_val() const {
		return val;
	}

	const Matrix<GF1<p>>& get_lambda() const {
		return lambda;
	}

	bool operator==(const GF_Mat<p>& other) const {
		return val == other.val;
	}

	void to_identity() {
		val.to_identity();
	}

	Polynomial<GF1<p>> to_poly() const {
		return matrix_to_poly(val);
	}

	Polynomial<GF1<p>> get_lambda_poly() const {
		return matrix_to_poly(lambda);
	}

	std::string to_string() const {
		std::string out;
		out.reserve(256);
		out += matrix_to_poly(val).to_string();
		return out;
	}
public:
	static GF_Mat<p> zero(const void * other_data) {
		return { *static_cast<Polynomial<GF1<p>>*>(other_data) };
	}

	static GF_Mat<p> one(const void * other_data) {
		return { *static_cast<Polynomial<GF1<p>>*>(other_data), { 1 } };
	}

	static Polynomial<GF1<p>> matrix_to_poly(const Matrix<GF1<p>>& mat) {
		std::vector<GF1<p>> coeffs;
		coeffs.reserve(mat.get_num_rows());
		size_t col = mat.get_num_cols() - 1;
		for (size_t row = 0; row < mat.get_num_rows(); row++) {
			coeffs.push_back(mat(row, col));
		}
		std::reverse(coeffs.begin(), coeffs.end());
		return coeffs;
	}
private:
	std::shared_ptr<void> irreducible_poly_ptr;
	Matrix<GF1<p>> val;
	Matrix<GF1<p>> lambda;
};

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Mat_Base, T>, bool> = true>
constexpr T zero(const void * other_data = nullptr) {
	return T::zero(other_data);
}

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Mat_Base, T>, bool> = true>
constexpr T one(const void * other_data = nullptr) {
	return T::one(other_data);
}

template<unsigned int p>
std::ostream& operator<<(std::ostream& s, const GF_Mat<p>& value) {
	s << value.to_poly();
	return s;
}

}
