#pragma once
#include "finite-field.hpp"
#include "matrix.hpp"
#include <algorithm>
#include <stdexcept>

class GF_Mat_Base {};

template<unsigned int p>
class GF_Mat : public Division_Ring<GF_Mat<p>>, public GF_Mat_Base {
public:
	GF_Mat() {}
	GF_Mat(const Polynomial<GF1<p>>& irreducible) : val{GF1<p>{0}, irreducible.degree()}, lambda{GF1<p>{0}, irreducible.degree()} {
		lambda.to_nilpotent_diag(1);
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

	GF_Mat operator+(const GF_Mat& other) const override {
		check_if_same_polynomial(other);
		GF_Mat out{ *this };
		out.val += other.val;
		return out;
	}

	GF_Mat& operator+=(const GF_Mat& other) override {
		check_if_same_polynomial(other);
		val += other.val;
		return *this;
	}

	GF_Mat operator-() const override {
		GF_Mat out{ *this };
		out.val = -out.val;
		return out;
	}

	GF_Mat operator-(const GF_Mat& other) const override {
		check_if_same_polynomial(other);
		GF_Mat out{ *this };
		out.val -= other.val;
		return out;
	}

	void negate() override {
		val.negate();
	}

	GF_Mat& operator-=(const GF_Mat& other) override {
		check_if_same_polynomial(other);
		val -= other.val;
		return *this;
	}

	GF_Mat operator*(const GF_Mat& other) const override {
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

	GF_Mat& operator*=(const GF_Mat& other) override {
		check_if_same_polynomial(other);
		val *= other.val;
		return *this;
	}

	GF_Mat& operator*=(const GF1<p>& other) {
		val *= other;
		return *this;
	}

	GF_Mat operator/(const GF_Mat& other) const override {
		check_if_same_polynomial(other);
		return (*this) * other.inv();
	}

	GF_Mat& operator/=(const GF_Mat& other) override {
		check_if_same_polynomial(other);
		(*this) *= other.inv();
		return *this;
	}

	GF_Mat operator^(int pow) const override {
		GF_Mat out{*this};
		out.val = out.val^pow;
		return out;
	}

	GF_Mat pow_u(unsigned int pow) const override {
		GF_Mat out{*this};
		out.val = out.val.pow_u(pow);
		return out;
	}

	GF_Mat inv() const override {
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

	static GF_Mat zero(const Polynomial<GF1<p>>& poly) {
		return {
			poly
		};
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
		std::vector<GF1<p>> coeffs;
		coeffs.reserve(val.get_num_rows());
		size_t col = val.get_num_cols() - 1;
		for (size_t row = 0; row < val.get_num_rows(); row++) {
			coeffs.push_back(val(row, col));
		}
		std::reverse(coeffs.begin(), coeffs.end());
		return coeffs;
	}

	const static Polynomial<GF1<p>>* literally_just_to_store_type_data() {
		static Polynomial<GF1<p>> poly;
		return &poly;
	}
private:
	Matrix<GF1<p>> val;
	Matrix<GF1<p>> lambda;
};

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Mat_Base, T>, bool> = true>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	decltype(T::literally_just_to_store_type_data()) irreducible = static_cast<decltype(T::literally_just_to_store_type_data())>(other_data);
	T out{*irreducible};
	return out;
}

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Mat_Base, T>, bool> = true>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	decltype(T::literally_just_to_store_type_data()) irreducible = static_cast<decltype(T::literally_just_to_store_type_data())>(other_data);
	T out{*irreducible};
	out.to_identity();
	return out;
}

template<unsigned int p>
std::ostream& operator<<(std::ostream& s, const GF_Mat<p>& value) {
	s << value.to_poly();
	return s;
}

