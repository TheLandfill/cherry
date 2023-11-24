#define SHOW_MATRIX_STEPS
#include "../include/gf-matrix.hpp"

using namespace cherry;

int main() {
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	Matrix<float> m1(0.0f, 3, 4);
	uint32_t val = 0b10101011101110101010101101010111;
	for (size_t row = 0; row < m1.get_num_rows(); row++) {
		for (size_t col = 0; col < m1.get_num_cols(); col++) {
			m1(row, col) = val & 0xff;
			val ^= val << 13;
			val ^= val >> 17;
			val ^= val << 5;
		}
	}
	auto r1 = m1.rref();
	std::cout << "----------\n";
	std::cout << m1;
	std::cout << r1;
	Matrix<GF1<5>> m2(0.0f, 3, 4);
	val = 0b10101011101110101010101101010111;
	for (size_t row = 0; row < m2.get_num_rows(); row++) {
		for (size_t col = 0; col < m2.get_num_cols(); col++) {
			m2(row, col) = val & 0xff;
			val ^= val << 13;
			val ^= val >> 17;
			val ^= val << 5;
		}
	}
	auto r2 = m2.rref();
	std::cout << "----------\n";
	std::cout << m2;
	std::cout << r2;
	return 0;
}
