#include "../include/numerical-integrator.hpp"
#include <cmath>
#include <format>
#include <iostream>

using namespace cherry::numerics;

template<size_t N>
cherry::Vector<1, double> nd_gaussian(const cherry::Vector<N, double>& in) {
	return cherry::Vector<1, double>{std::exp(-in.sqr_magnitude())};
}

struct Name_And_Offsets {
	const char * name;
	double offset;
};

template<size_t N>
void simple_sums_over_k_cell() {
	std::array<Name_And_Offsets, 3> sum_names {
		Name_And_Offsets{
			"Left",
			0.0
		},
		{
			"Right",
			1.0
		},
		{
			"Midpoint",
			0.5
		}
	};
	for (const Name_And_Offsets& nao : sum_names) {
		std::cout << nao.name << "\n";
		for (size_t log_n = 0; log_n < 20 / N; log_n++) {
			cherry::Vector<N, double> start, end;
			cherry::Vector<N, size_t> num;
			std::fill(start.x, start.x + N, 0.0);
			std::fill(end.x, end.x + N, 1.0);
			std::fill(num.x, num.x + N, static_cast<size_t>(1) << log_n);
			Interval iv{start, end};
			Regular_Input_Value_Generator<N> rvig{iv, num, nao.offset};
			double out = n_integrate<
				decltype(nd_gaussian<N>),
				N,
				cherry::Vector<N, double>,
				cherry::Vector<1, double>
			>(rvig, nd_gaussian<N>)[0];
			std::cout << std::format("    n = {:9}: {}"/*"{:30x}"*/"\n", 1 << log_n, out/*, *(long long*)(&out) */);
		}
	}
}

int main() {
	simple_sums_over_k_cell<2>();
}
