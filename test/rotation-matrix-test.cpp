#include "test-one-possibility.hpp"
#include "../include/gf-matrix.hpp"

int main() {
	using namespace cherry;
	Matrix<float> v{0.0f, 3, 1};
	v(0, 0) = 218.0f;
	v(1, 0) = -12.0f;
	v(2, 0) = -134.0f;
	Matrix<float> R{0.0f, 3, 3};
	std::vector<float> data{
		0.9658319, -0.0202837,  0.2583743,
		0.0646098,  0.9843011, -0.1642462,
		-0.2509867,  0.1753277,  0.9519800
	};
	for (size_t row = 0; row < 3; row++) {
		for (size_t col = 0; col < 3; col++) {
			R(row, col) = data[3 * row + col];
		}
	}
	std::cout << R * v << "\n";
	return 0;
}
