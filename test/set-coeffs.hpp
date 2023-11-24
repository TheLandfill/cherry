#pragma once
#include "../include/gf.hpp"

template<unsigned int p>
void set_coeffs(uint64_t val, std::vector<cherry::GF1<p>>& coeffs) {
	coeffs.clear();
	do {
		coeffs.push_back(val % p);
		val /= p;
	} while (val > 0);
}
