#include "../include/gf.hpp"

using namespace cherry;

int main() {
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	GF<5, GF_Mat<5>> p1{irreducible_poly, {{1, 2, 4}}};
	GF<5, GF_Mat<5>> p2{irreducible_poly, {{1, 1, 3}}};
	std::cout << (p1 + p2) << "\n";
	std::cout << (p1 * p2) << "\n";
	return 0;
}
