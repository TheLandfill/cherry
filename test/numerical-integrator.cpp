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

template<size_t N>
void simple_sums_over_k_cell() {
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

void trapezoid_sums_over_interval() {
	std::cout << "Trapezoid\n";
	for (size_t log_n = 0; log_n < 20; log_n++) {
		cherry::Vector<1, double> start{0.0}, end{1.0};
		cherry::Vector<1, size_t> num{(static_cast<size_t>(1) << log_n)};
		Interval iv{start, end};
		Trapezoid_Rule_Value_Generator trvg{iv, num};
		Regular_Input_Value_Generator<1> rvig{iv, num, 1.0};
		Regular_Input_Value_Generator<1> lvig{iv, num, 0.0};
		const auto& n_int = n_integrate<
			decltype(nd_gaussian<1>),
			1,
			cherry::Vector<1, double>,
			cherry::Vector<1, double>,
			double,
			decltype(rvig)
		>;
		const auto& n_int_t = n_integrate<
			decltype(nd_gaussian<1>),
			1,
			cherry::Vector<1, double>,
			cherry::Vector<1, double>,
			double,
			decltype(trvg)
		>;
		double r_out = n_int(rvig, nd_gaussian<1>)[0];
		double l_out = n_int(lvig, nd_gaussian<1>)[0];
		double t_out = n_int_t(trvg, nd_gaussian<1>)[0];
		double at_out = l_out + r_out;
		at_out *= 0.5;
		std::cout << std::format("    n = {:9}: {:.10} - {:.10} = {:.10}" "\n", 1 << log_n, t_out, at_out, t_out - at_out);
	}
}

int main() {
	// simple_sums_over_k_cell<2>();
	trapezoid_sums_over_interval();
}
