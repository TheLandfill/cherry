#pragma once
#include "division-ring.hpp"
#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <format>
#include <iostream>

namespace cherry {

class Matrix_Base {};

// Note that I'm classifying a matrix as a division ring even though not every
// element has an inverse because I want the operation of division to be
// available in the class.
template<typename T>
class Matrix : public Division_Ring<Matrix<T>>, public Matrix_Base {
public:
	Matrix() {}
	Matrix(const T& fill, size_t rows) : num_rows(rows), num_cols(rows), data(num_rows * num_cols, fill) {}

	Matrix(const T& fill, size_t rows, size_t cols) : num_rows(rows), num_cols(cols), data(num_rows * num_cols, fill) {}

	T& operator()(size_t r, size_t c) {
		return data[r * num_cols + c];
	}

	T operator()(size_t r, size_t c) const {
		return data[r * num_cols + c];
	}

	Matrix<T> operator+(const Matrix<T>& other) const override {
		check_addition_size(other, "add");
		Matrix<T> out{zero<T>(), num_rows, num_cols};
		for (size_t i = 0; i < data.size(); i++) {
			T& c = out.data[i];
			const T& a = data[i];
			const T& b = other.data[i];
			c = a + b;
		}
		return out;
	}

	Matrix<T>& operator+=(const Matrix<T>& other) override {
		check_addition_size(other, "add");
		for (size_t i = 0; i < data.size(); i++) {
			T& a = data[i];
			const T& b = other.data[i];
			a += b;
		}
		return *this;
	}

	Matrix<T> operator-(const Matrix<T>& other) const override {
		check_addition_size(other, "subtract");
		Matrix<T> out{zero<T>(), num_rows, num_cols};
		for (size_t i = 0; i < data.size(); i++) {
			T& c = out.data[i];
			const T& a = data[i];
			const T& b = other.data[i];
			c = a - b;
		}
		return out;
	}

	Matrix<T>& operator-=(const Matrix<T>& other) override {
		check_addition_size(other, "subtract");
		for (size_t i = 0; i < data.size(); i++) {
			T& a = data[i];
			const T& b = other.data[i];
			a -= b;
		}
		return *this;
	}

	Matrix<T> operator*(const Matrix<T>& other) const override {
		check_multiplication_size(other);
		Matrix<T> out{zero<T>(), num_rows, other.num_cols};
		for (size_t i = 0; i < num_rows; i++) {
			for (size_t j = 0; j < other.num_cols; j++) {
				for (size_t k = 0; k < num_cols; k++) {
					out(i, j) += (*this)(i, k) * other(k, j);
				}
			}
		}
		return out;
	}

	Matrix<T>& operator*=(const Matrix<T>& other) override {
		*this = (*this * other);
		return *this;
	}

	Matrix<T> operator-() const override {
		Matrix<T> out{zero<T>(), num_rows, num_cols};
		for (size_t i = 0; i < data.size(); i++) {
			T& a = out.data[i];
			const T& b = data[i];
			a = -b;
		}
		return out;
	}

	void negate() override {
		for (size_t i = 0; i < data.size(); i++) {
			data[i] = -data[i];
		}
	}

	Matrix<T> pow_u(unsigned int pow) const override {
		check_square("Exponentiation");
		Matrix<T> out{zero<T>(), num_rows};
		out.to_identity();
		Matrix<T> val_pow{ *this };
		while (pow > 0) {
			if (pow & 1) {
				out *= val_pow;
			}
			val_pow *= val_pow;
			pow >>= 1;
		}
		return out;
	}

	Matrix<T> operator^(int pow) const override {
		if (pow < 0) {
			return inv().pow_u(static_cast<unsigned int>(std::abs(pow)));
		} else {
			return pow_u(static_cast<unsigned int>(std::abs(pow)));
		}
	}

	size_t get_num_rows() const {
		return num_rows;
	}

	size_t get_num_cols() const {
		return num_cols;
	}

	Matrix<T> operator/(const Matrix<T>& other) const override {
		return (*this) * other.inv();
	}

	Matrix<T>& operator/=(const Matrix<T>& other) override {
		*this *= other.inv();
		return *this;
	}

	Matrix<T> inv() const override {
		check_square("Inversion");
		Matrix<T> dummy{*this};
		T zero_v = zero<T>();
		T one_v = one<T>();
		Matrix<T> inverse{zero_v, num_rows};
		inverse.to_identity();
		#ifdef SHOW_MATRIX_STEPS
		std::cout << dummy << inverse;
		#endif
		for (size_t col = 0; col < num_cols; col++) {
			size_t actual_row = col;
			size_t cur_row = actual_row + 1;
			T& cur_val = dummy(actual_row, col);
			if (cur_val == zero_v) {
				while (cur_row < num_rows && dummy(cur_row, col) == zero_v) {
					cur_row++;
				}
				if (dummy(cur_row, col) == zero_v) {
					throw std::domain_error("Matrix does not have an inverse.");
				}
				dummy.ero_swap(cur_row, actual_row);
				inverse.ero_swap(cur_row, actual_row);
				cur_val = dummy(actual_row, col);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Swapping Rows " << cur_row << " and " << actual_row << "\n";
				std::cout << dummy << inverse;
				#endif
			}
			if (cur_val != one_v) {
				const T& scale = dummy(actual_row, col);
				T inverse_scale = cherry::inv(scale);
				dummy.ero_scale(actual_row, inverse_scale, col);
				inverse.ero_scale(actual_row, inverse_scale);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Scale row " << actual_row << " by " << inverse_scale << "\n";
				std::cout << dummy << inverse;
				#endif
			}
			for (size_t row = 0; row < col; row++) {
				T dummy_val = dummy(row, col);
				dummy.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val, col);
				inverse.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Subtract row " << dummy_val << " * R" << actual_row << " from R" << row << "\n";
				std::cout << dummy << inverse;
				#endif
			}
			for (size_t row = col + 1; row < num_rows; row++) {
				T dummy_val = dummy(row, col);
				dummy.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val, col);
				inverse.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Subtract row " << dummy_val << " * R" << actual_row << " from R" << row << "\n";
				std::cout << dummy << inverse;
				#endif
			}
		}
		return inverse;
	}

	Matrix<T> rref() const {
		Matrix<T> dummy{*this};
		T zero_v = zero<T>();
		T one_v = one<T>();
		#ifdef SHOW_MATRIX_STEPS
		std::cout << "Matrix is " << num_rows << "x" << num_cols << "\n";
		std::cout << dummy;
		#endif
		for (size_t col = 0; col < std::min(num_cols, num_rows); col++) {
			#ifdef SHOW_MATRIX_STEPS
			std::cout << "On column " << col << "\n";
			#endif
			size_t actual_row = col;
			size_t cur_row = actual_row + 1;
			T& cur_val = dummy(actual_row, col);
			#ifdef SHOW_MATRIX_STEPS
			std::cout << "M[" << actual_row << "][" << col << "] = " << cur_val << "\n";
			#endif
			if (cur_val == zero_v) {
				if (cur_row >= num_rows) {
					continue;
				}
				while (cur_row < num_rows && dummy(cur_row, col) == zero_v) {
					cur_row++;
				}
				if (dummy(cur_row, col) == zero_v) {
					std::cout << "Matrix is incomplete\n";
					return dummy;
				}
				dummy.ero_swap(cur_row, actual_row);
				cur_val = dummy(actual_row, col);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Swapping Rows " << cur_row << " and " << actual_row << "\n";
				std::cout << dummy;
				#endif
			}
			if (cur_val != one_v) {
				const T& scale = dummy(actual_row, col);
				T inverse_scale = cherry::inv(scale);
				dummy.ero_scale(actual_row, inverse_scale, col);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Scale row " << actual_row << " by " << inverse_scale << "\n";
				std::cout << dummy;
				#endif
			}
			for (size_t row = 0; row < col; row++) {
				T dummy_val = dummy(row, col);
				dummy.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val, col);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Subtract row " << dummy_val << " * R" << actual_row << " from R" << row << "\n";
				std::cout << dummy;
				#endif
			}
			for (size_t row = col + 1; row < num_rows; row++) {
				T dummy_val = dummy(row, col);
				dummy.ero_subtract_scaled_row_from_r1(row, actual_row, dummy_val, col);
				#ifdef SHOW_MATRIX_STEPS
				std::cout << "Subtract row " << dummy_val << " * R" << actual_row << " from R" << row << "\n";
				std::cout << dummy;
				#endif
			}
		}
		return dummy;
	}

	void to_zero() {
		T val_to_set = zero<T>();
		std::fill(data.begin(), data.end(), val_to_set);
	}

	void to_identity() {
		to_zero();
		T val_to_set = one<T>();
		for (size_t i = 0; i < std::min(num_rows, num_cols); i++) {
			(*this)(i, i) = val_to_set;
		}
	}

	void to_identity(T val_to_set) {
		to_zero();
		for (size_t i = 0; i < std::min(num_rows, num_cols); i++) {
			(*this)(i, i) = val_to_set;
		}
	}

	void to_nilpotent_diag(int start, T val_to_set) {
		to_zero();
		if (start >= 0) {
			for (size_t i = start; i < num_cols; i++) {
				(*this)(i - start, i) = val_to_set;
			}
		} else {
			for (size_t i = std::abs(start); i < num_rows; i++) {
				(*this)(i, i - start) = val_to_set;
			}
		}
	}

	void to_nilpotent_diag(int start) {
		to_zero();
		T val_to_set = one<T>();
		if (start >= 0) {
			for (size_t i = start; i < num_cols; i++) {
				(*this)(i - start, i) = val_to_set;
			}
		} else {
			for (size_t i = std::abs(start); i < num_rows; i++) {
				(*this)(i, i - start) = val_to_set;
			}
		}
	}

	void to_circulant_diag(size_t start, T val_to_set) {
		to_zero();
		for (size_t i = 0; i < num_rows; i++) {
			(*this)(i, (i + start) % num_cols) = val_to_set;
		}
	}

	void to_circulant_diag(size_t start) {
		to_zero();
		T val_to_set = one<T>();
		for (size_t i = 0; i < num_rows; i++) {
			(*this)(i, (i + start) % num_cols) = val_to_set;
		}
	}

	void ero_scale(size_t row, const T& scale, size_t start_col = 0) {
		for (size_t i = start_col; i < num_cols; i++) {
			(*this)(row, i) *= scale;
		}
	}

	void ero_swap(size_t r1, size_t r2) {
		T temp;
		for (size_t i = 0; i < num_cols; i++) {
			temp = (*this)(r1, i);
			(*this)(r1, i) = (*this)(r2, i);
			(*this)(r2, i) = temp;
		}
	}

	void ero_subtract_scaled_row_from_r1(size_t r1, size_t r2, const T& scale, size_t start_col = 0) {
		for (size_t i = start_col; i < num_cols; i++) {
			(*this)(r1, i) -= scale * (*this)(r2, i);
		}
	}

	Matrix<T> operator*(const T& scalar) const {
		Matrix<T> out{*this};
		for (auto& element : out.data) {
			element *= scalar;
		}
		return out;
	}

	Matrix<T>& operator*=(const T& scalar) {
		for (auto& element : data) {
			element *= scalar;
		}
		return *this;
	}

	bool operator==(const Matrix<T>& other) const {
		check_addition_size(other, "Equality testing");
		for (size_t i = 0; i < data.size(); i++) {
			if (data[i] != other.data[i]) {
				return false;
			}
		}
		return true;
	}

	T trace() const {
		T out{ zero<T>() };
		for (size_t i = 0; i < num_rows; i++) {
			out += (*this)(i, i);
		}
		return out;
	}
public:
	const static T* literally_just_to_store_type_data() {
		static T var;
		return &var;
	}

private:
	void check_addition_size(const Matrix<T>& other, const char * operation) const {
		if (num_rows != other.num_rows || num_cols != other.num_cols) {
			throw std::out_of_range(
				std::format(
					"Trying to {} a matrix of dimensions {}x{} "
					"to a matrix of dimensions {}x{}.",
					operation,
					num_rows,
					num_cols,
					other.num_rows,
					other.num_cols
				)
			);
		}
	}

	void check_multiplication_size(const Matrix<T>& other) const {
		if (num_cols != other.num_rows) {
			throw std::out_of_range(
				std::format(
					"Trying to multiply a matrix of dimensions {}x{} "
					"with a matrix of dimensions {}x{}.",
					num_rows,
					num_cols,
					other.num_rows,
					other.num_cols
				)
			);
		}
	}

	void check_square(const char * operation) const {
		if (num_rows != num_cols) {
			throw std::invalid_argument(
				std::format(
					"{} only works with square matrices, but this "
					"matrix has dimensions of {}x{}.",
					operation,
					num_rows,
					num_cols
				)
			);
		}
	}
private:
	size_t num_rows;
	size_t num_cols;
	std::vector<T> data;
};

template<typename T, std::enable_if_t<std::is_base_of_v<Matrix_Base, T>, bool> = true>
constexpr T zero(T * literally_just_type_info = nullptr, void * other_data = nullptr) {
	(void)literally_just_type_info;
	size_t dim = *static_cast<size_t*>(other_data);
	Matrix<T> out{zero<T>(T::literally_just_to_store_type_data()), dim};
	return out;
}

template<typename T, std::enable_if_t<std::is_base_of_v<Matrix_Base, T>, bool> = true>
constexpr T one(T * literally_just_type_info = nullptr, void * other_data = nullptr) {
	size_t dim = *static_cast<size_t*>(other_data);
	(void)literally_just_type_info;
	Matrix<T> out{zero<T>(), dim};
	out.to_identity();
	return out;
}

template<typename T>
Matrix<T> operator*(const T& scalar, const Matrix<T>& matrix) {
	return matrix * scalar;
}

template<typename T>
std::ostream& operator<<(std::ostream& s, const Matrix<T>& matrix) {
	for (size_t row = 0; row < matrix.get_num_rows(); row++) {
		for (size_t col = 0; col < matrix.get_num_cols(); col++) {
			s << matrix(row, col) << " ";
		}
		s << "\n";
	}
	s << "\n";
	return s;
}

}
