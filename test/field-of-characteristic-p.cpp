#include "test-one-possibility.hpp"
#include "../include/gf-matrix.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Field_Of_Characteristic_P_Test {
public:
	using GFs = GF<p, Repr>;
	bool operator()(const GFs& x, const Polynomial<GF1<p>> * poly) {
		GFs gf0 = zero(&x, static_cast<const void*>(poly));
		GFs y{ x };
		for (unsigned int i = 1; i < p; i++) {
			y += x;
		}
		// fail += 8;
		if (!(y == gf0) || fail == 0) {
			std::cerr << " x: " << x << "\n";
			std::cerr << p << "x: " << y << "\n";
			std::cerr << "--------------------\n";
			return false;
		} else {
			return true;
		}
	}
private:
	uint8_t fail = 8;
};

int main() {
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	Field_Of_Characteristic_P_Test<5, GF_Mat<5>> focpt;
	run_function_on_all_single_cases<5, GF_Mat<5>>(
		irreducible_poly,
		"Field Of Characteristic P Test in GF(5^3)",
		focpt
	);
}
