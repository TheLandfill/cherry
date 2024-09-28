#include "../include/gf-matrix.hpp"
#include "test-two-possibilities.hpp"
#include "simple-timer.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Babylonian_Method {
public:
	using GFs = GF<p, Repr>;
	Babylonian_Method(const GFs& z, const GFs two_i) :
		this_zero(z),
		two_inv(two_i)
	{
		prev_seen.reserve(p * p);
	}
	bool operator()(const GFs& x, GFs prev) {
		if (x == this_zero) {
			return true;
		}
		prev_seen.clear();
		GFs x_sqr{x};
		x_sqr *= x;
		GFs next = prev;
		while (prev != this_zero && std::find(prev_seen.rbegin(), prev_seen.rend(), next) == prev_seen.rend()) {
			prev_seen.push_back(next);
			GFs test = x_sqr / prev;
			test += prev;
			next = two_inv * test;
			if (next * next == x_sqr) {
				return true;
			}
			prev = next;
		}
		return false;
	}
private:
	GFs this_zero;
	GFs two_inv;
	std::vector<GFs> prev_seen;
};

int main() {
	Timer t(__FILE__);
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	Polynomial<GF1<5>> irreducible_poly{{3, 0, 1}};
	GF<5, GF_Mat<5>> two_inv{irreducible_poly, {{ 2 }}};
	two_inv = two_inv.inv();
	Babylonian_Method<5, GF_Mat<5>> bm(
		zero<GF<5, GF_Mat<5>>>(&irreducible_poly),
		two_inv
	);
	run_function_on_all_double_cases<5, GF_Mat<5>>(
		irreducible_poly,
		"Babylonian_Method in GF(5^2)",
		bm
	);
	return 0;
}
