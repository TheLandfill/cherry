#include "test-one-possibility.hpp"
#include "../include/gf-matrix.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Fermats_Little_Theorem_Single_Test {
public:
	using GFs = GF<p, Repr>;
	bool operator()(const GFs& x, const Polynomial<GF1<p>>* poly) {
		if (x == zero(&x, poly)) {
			return true;
		}
		GFs to_negative_1 = x^-1;
		// std::cout << "\n";
		// std::cout << to_negative_1.get_val().get_val();
		// std::cout << (x * to_negative_1).get_val().get_val();
		GFs inverse = x.inv();
		// std::cout << inverse.get_val().get_val();
		// std::cout << (x * inverse).get_val().get_val();
		uint64_t power = 1;
		uint64_t val_pow = p;
		uint64_t degree = x.get_order();
		while (degree > 0) {
			if (degree & 1) {
				power *= val_pow;
			}
			val_pow *= val_pow;
			degree >>= 1;
		}
		power -= 2;
		GFs to_p_to_n_minus_two = x.pow_u(power);
		// std::cout << to_p_to_n_minus_two.get_val().get_val();
		// std::cout << (to_p_to_n_minus_two * x).get_val().get_val();
		// std::cout << "----------------------------------------\n";
		if (
			!(to_p_to_n_minus_two == to_negative_1) ||
			!(to_p_to_n_minus_two == inverse)
		) {
			std::cerr << "          x: " << x << "\n";
			std::cerr << "       x^-1: " << to_negative_1 << "\n";
			std::cerr << "    x.inv(): " << inverse << "\n";
			std::cerr << "x^(p^n - 2): " << to_p_to_n_minus_two << "\n";
			std::cerr << "------------------------\n";
			return false;
		} else {
			return true;
		}
	}
};

int main() {
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	Fermats_Little_Theorem_Single_Test<5, GF_Mat<5>> fltst;
	run_function_on_all_single_cases<5, GF_Mat<5>>(
		irreducible_poly,
		"Fermat's Little Theorem in GF(5^3)",
		fltst
	);
}
