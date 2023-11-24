#define SHOW_MATRIX_STEPS
#include "test-one-possibility.hpp"
#include "../include/gf-matrix.hpp"

using namespace cherry;

int main() {
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	GF<5> a{irreducible_poly, {{2, 3, 1}}};
	auto b = a.inv();
	std::cout << (a * b).get_val().get_val();
	return 0;
}
