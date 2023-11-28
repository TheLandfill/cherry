#include "set-coeffs.hpp"
#include "simple-timer.hpp"

using namespace cherry;

template<unsigned int p, typename Repr>
class Polynomial_Test {
public:
	using GFs = GF<p, Repr>;
	Polynomial_Test(
		const Polynomial<GF1<p>>& poly
	) :
		irreducible_poly(poly),
		zero_element(zero<GFs>(&irreducible_poly))
	{}

	void find_roots(const Polynomial<GF1<p>>& poly) {
		std::cout << poly << "\n"
		"----------------------------------------\n";
		roots.clear();
		std::vector<GF1<p>> x_coeffs;
		x_coeffs.reserve(irreducible_poly.degree());
		uint64_t num_elements = p;
		for (size_t i = 1; i < irreducible_poly.degree(); i++) {
			num_elements *= p;
		}
		std::cout << "num_elements: " << num_elements << "\n";
		for (uint64_t i = 0; i < num_elements; i++) {
			set_coeffs(i, x_coeffs);
			GFs x{irreducible_poly, Polynomial<GF1<p>>{x_coeffs}};
			if (poly.eval(x, &irreducible_poly) == zero_element) {
				roots.push_back(x);
				std::cout << x << "\n";
			}
		}
		std::cout << "roots: " << roots.size() << "\n";
		std::vector<GFs> symmetric_poly_terms;
		uint8_t num_bits = 2 << (poly.degree() - 1);
		std::cout << "num_bits: " << static_cast<int>(num_bits) << "\n";
		for (uint8_t i = 0; i < num_bits; i++) {
			uint8_t val = i;
			if (val == 0) {
				symmetric_poly_terms.emplace_back(irreducible_poly);
				continue;
			} else {
				symmetric_poly_terms.emplace_back(irreducible_poly, Polynomial<GF1<5>>{{1}});
			}
			std::cout << (int)val << "\n";
			for (const auto& sym_poly_term : symmetric_poly_terms) {
				std::cout << sym_poly_term << "\n";
			}
			uint8_t cur_poly = 0;
			while (val > 0) {
				if (val & 1) {
					std::cout << "val:      " << (int)val << "\n";
					std::cout << "cur_poly: " << (int)cur_poly << "\n";
					std::cout << "before:   " << symmetric_poly_terms.back() << "\n";
					symmetric_poly_terms.back() *= roots[cur_poly];
					std::cout << "after:    " << symmetric_poly_terms.back() << "\n";
				}
				val >>= 1;
				cur_poly++;
			}
		}
		std::vector<GFs> coeffs;
		for (uint8_t i = 1; i <= poly.degree(); i++) {
			coeffs.emplace_back(irreducible_poly);
			for (uint8_t j = 0; j < num_bits; j++) {
				if (std::popcount(j) == i) {
					coeffs.back() += symmetric_poly_terms[j];
				}
			}
			if (i % 2 == 1) {
				coeffs.back().negate();
			}
		}
		for (const auto& coeff : coeffs) {
			std::cout << coeff << "\n";
		}
		std::cout << "\n----------------------------------------\n";
	}
private:
	std::vector<GFs> roots;
	Polynomial<GF1<p>> irreducible_poly;
	GFs zero_element;
};

int main() {
	Timer t(__FILE__);
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 2, 3, 3, 4, 1}};
	Polynomial<GF1<5>> poly{{3, 2, 4, 1}};
	Polynomial_Test<5, GF_Mat<5>> pt(irreducible_poly);
	pt.find_roots(poly);
}
