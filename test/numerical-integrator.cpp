#include "../include/numerical-integrator.hpp"
#include <cmath>
#include <format>
#include <iostream>

using namespace cherry::numerics;

double gaussian(double in) {
	in *= -in;
	return exp(in);
}

template<size_t N>
double nd_gaussian(const std::array<double, N>& in) {
	double total = 0.0;
	for (const double n : in) {
		total -= n * n;
	}
	return exp(total);
}

struct Name_And_Offsets {
	const char * name;
	double offset;
};

void simple_sums_over_an_interval() {
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
		for (size_t log_n = 0; log_n < 20; log_n++) {
			Interval iv{0.0, 1.0, static_cast<size_t>(1) << log_n};
			std::array<Interval<double>, 1> ivs{iv};
			Regular_Input_Value_Generator rvig{ivs, nao.offset};
			std::cout << std::format("    n = {:9}: {}\n", 1 << log_n, n_integrate(rvig, gaussian));
		}
	}
}

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
		for (size_t log_n = 0; log_n < 20; log_n++) {
			Interval iv{0.0, 1.0, static_cast<size_t>(1) << log_n};
			std::array<Interval<double>, N> ivs{iv};
			std::fill(ivs.begin(), ivs.end(), iv);
			Regular_Input_Value_Generator rvig{ivs, nao.offset};
			std::cout << std::format("    n = {:9}: {}\n", 1 << log_n, n_integrate(rvig, nd_gaussian<N>));
		}
	}
}

int main() {
	simple_sums_over_an_interval();
	simple_sums_over_k_cell<1>();
}
