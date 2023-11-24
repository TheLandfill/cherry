#include "../include/finite-field.hpp"
#include "../include/gf-matrix.hpp"
#include "test-two-possibilities.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Freshmans_Dream_Single_Test {
public:
	using GFs = GF<p, Repr>;
	bool operator()(const GFs& x, const GFs& y) {
		GFs power_of_sum = (x + y)^p;
		GFs sum_of_power = (x^p) + (y^p);
		//fail++;
		if (!(power_of_sum == sum_of_power) || fail == 0) {
			std::cerr << "        x: " << x << "\n";
			std::cerr << "        y: " << y << "\n";
			std::cerr << "    x + y: " << x + y << "\n";
			std::cerr << "x^p + y^p: " << sum_of_power << "\n";
			std::cerr << "(x + y)^p: " << power_of_sum << "\n";
			std::cerr << "--------------------\n";
			return false;
		} else {
			return true;
		}
	}
private:
	uint8_t fail = 1;
};

int main() {
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	Freshmans_Dream_Single_Test<5, GF_Mat<5>> btst;
	run_function_on_all_double_cases<5, GF_Mat<5>>(
		irreducible_poly,
		"Freshmans_Dream_Single_Test in GF(5^3)",
		btst
	);
}
