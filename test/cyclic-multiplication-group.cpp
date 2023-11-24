#include "test-one-possibility.hpp"
#include "../include/gf-matrix.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Cyclic_Multiplication_Group_Test {
public:
	using GFs = GF<p, Repr>;
	bool operator()(const GFs& x, const Polynomial<GF1<p>> * poly) {
		GFs gf1 = one(&x, static_cast<const void*>(poly));
		GFs gf0 = zero(&x, static_cast<const void*>(poly));
		if (x == gf0) {
			return false;
		}
		GFs val = x;
		uint64_t count = 1;
		while (!(val == gf1)) {
			val *= x;
			count++;
		}
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
		power -= 1;
		std::cout << x << " -> " << count << "\n";
		return count == power;
	}
};

int main() {
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
	Cyclic_Multiplication_Group_Test<5, GF_Mat<5>> cmgt;
	run_function_on_all_single_cases<5, GF_Mat<5>>(
		irreducible_poly,
		"Cyclic Multiplication Group Test in GF(5^3)",
		cmgt
	);
}
