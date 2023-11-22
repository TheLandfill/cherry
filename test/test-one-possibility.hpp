#pragma once
#include "finite-field.hpp"
#include "set-coeffs.hpp"
#include <iostream>
#include <iomanip>

// This function should work up to a max of 3^20, 5^13, 7^11, 11^9, etc. The
// max degree of irreducible_poly should be floor(32 log(2) / log(5)), but that
// would take quite a while, so I'd probably divide all these degrees by two.
template<unsigned int p, typename Repr, typename Func>
void run_function_on_all_single_cases(
	const Polynomial<GF1<p>>& irreducible_poly,
	const char * test_name,
	Func f
) {
	using GFs = GF<p, Repr>;
	std::vector<GF1<p>> x_coeffs;
	x_coeffs.reserve(irreducible_poly.degree());
	uint64_t num_elements = p;
	uint64_t successes = 0;
	for (size_t i = 1; i < irreducible_poly.degree(); i++) {
		num_elements *= p;
	}
	uint64_t tries = num_elements;
	for (uint64_t i = 0; i < num_elements; i++) {
		set_coeffs(i, x_coeffs);
		GFs x{irreducible_poly, x_coeffs};
		bool succeeded = f(x, &irreducible_poly);
		successes += succeeded;
	}
	std::cout << test_name;
	if (successes == tries) {
		std::cout << " succeeded!\n";
	} else {
		uint64_t fails = tries - successes;
		std::cout << " failed on " << fails << " out of " << tries << " cases!\n";
		std::cout << "Failure rate: " << std::setprecision(4) << 100.0 * static_cast<double>(fails) / static_cast<double>(tries) << "%\n";
	}
}

