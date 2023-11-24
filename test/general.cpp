#include "../include/finite-field.hpp"
#include "../include/matrix.hpp"
#include "../include/gf-matrix.hpp"
#include <iostream>
#include <bit>

using namespace cherry;

int main() {
	GF1<5> a = 3;
	GF1<5> b = 4;
	std::cout <<
	"Variable Values\n"
	"----------------------------------------\n";
	std::cout << "a\t= " << a << "\n";
	std::cout << "b\t= " << b << "\n\n";
	std::cout <<
	"Arithmetic Operations\n"
	"----------------------------------------\n";
	std::cout << "a + b\t= " << a + b << "\n";
	std::cout << "a - b\t= " << a - b << "\n";
	std::cout << "a b\t= " << a * b << "\n";
	std::cout << "a / b\t= " << a / b << "\n\n";
	std::cout << "Powers of a\n"
	"----------------------------------------\n";
	for (int i = -5; i <= 5; i++) {
		std::cout << "a^" << i << "\t= " << (a^i) << "\n";
	}
	std::cout << "\n"
	"Powers of b\n"
	"----------------------------------------\n";
	for (int i = -5; i <= 5; i++) {
		std::cout << "b^" << i << "\t= " << (b^i) << "\n";
	}
	std::cout << one<GF1<5>>() << "\n";
	std::cout << zero<GF1<5>>() << "\n";
	const size_t mat_size = 23;
	Matrix<GF1<5>> m1{mat_size, mat_size};
	m1.to_circulant_diag(1);
	Matrix<GF1<5>> m = m1 + (m1^(mat_size - 1));
	m1.to_identity();
	m += m1;
	std::cout << m;
	std::cout << (m^(5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 - 1)) << "\n";
	// for (size_t i = 1; i < 125; i++) {
	// 	std::cout << "m^" << i << "\n";
	// 	std::cout << (m^i);
	// }
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	Polynomial<GF1<5>> other_poly{{2, 4, 1}};
	std::cout << irreducible_poly << "\n";
	std::cout << other_poly << "\n";
	std::cout << irreducible_poly + other_poly << "\n";
	Polynomial<float> regular_poly{{ 2.0f, -1.75f, 2.5f, 7.5f / 16.0f, 11.0f / 64.0f }};
	std::cout << regular_poly << "\n";
	Polynomial<GF1<5>> zero_poly = zero<
		Polynomial<GF1<5>>
	>();
	std::cout << zero_poly << "\n";
	Polynomial<GF1<5>> one_poly = one<
		Polynomial<GF1<5>>
	>();
	std::cout << one_poly << "\n";
	GF<5> element{irreducible_poly, other_poly};
	GF<5> zero_element{irreducible_poly};
	std::cout << element << "\n";
	std::cout << "element^2\n" << (element^2) << "\n";
	std::cout << "element^4\n" << (element^4) << "\n";
	std::cout << "element^31\n" << (element^31) << "\n";
	std::cout << "element^62\n" << (element^62) << "\n";
	std::cout << "element^61\n" << (element^61) << "\n";
	std::cout << "element^-1\n" << (element.inv()) << "\n";
	std::cout << "element^123\n" << (element^123) << "\n";
	std::cout << "element * element^-1\n" << (element * element.inv()) << "\n";
	std::cout << "element^124\n" << (element^124) << "\n";
	std::vector<GF<5>> roots;
	for (unsigned int a0 = 0; a0 < 5; a0++) {
		for (unsigned int a1 = 0; a1 < 5; a1++) {
			for (unsigned int a2 = 0; a2 < 5; a2++) {
				if (a0 == 0 && a1 == 0 && a2 == 0) {
					continue;
				}
				Polynomial<GF1<5>> test_poly{{a0, a1, a2}};
				GF<5> element{irreducible_poly, test_poly};
				if (irreducible_poly.eval(element, &irreducible_poly) == zero_element) {
					roots.push_back(element);
					std::cout << element << "\n";
				}
			}
		}
	}
	std::vector<GF<5>> symmetric_poly_terms;
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t val = i;
		if (val == 0) {
			symmetric_poly_terms.emplace_back(irreducible_poly);
			continue;
		} else {
			symmetric_poly_terms.emplace_back(irreducible_poly, Polynomial<GF1<5>>{{1}});
		}
		uint8_t cur_poly = 0;
		while (val > 0) {
			if (val & 1) {
				symmetric_poly_terms.back() *= roots[cur_poly];
			}
			val >>= 1;
			cur_poly++;
		}
	}
	std::vector<GF<5>> coeffs;
	for (uint8_t i = 1; i <= 3; i++) {
		coeffs.emplace_back(irreducible_poly);
		for (uint8_t j = 0; j < 8; j++) {
			if (std::popcount(j) == i) {
				coeffs.back() += symmetric_poly_terms[j];
			}
		}
		if (i % 2 == 1) {
			coeffs.back().negate();
		}
	}
	std::cout << irreducible_poly << "\n";
	for (const auto& coeff : coeffs) {
		std::cout << coeff << "\n";
	}
	// Matrix<GF<5, GF_mat<5>>> M{element, 2};
	return 0;
}
