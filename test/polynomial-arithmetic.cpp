#include "../include/gf.hpp"

using namespace cherry;

void test_floating_point_polynomials() {
	Polynomial<float> p{{1, 4, 5, 3}};
	Polynomial<float> q{{12, 32, 4, 1, 3, 3, 8, 9}};
	auto r_forward = p * q;
	auto r_reverse = q * p;
	auto should_be_zero = r_forward - r_reverse;
	auto s = p + q;
	auto t = p - q;
	auto u = s * t;
	auto v = p * p - q * q;
	auto should_also_be_zero = u - v;
	std::cout << p << "\n";
	std::cout << q << "\n";
	std::cout << r_forward << "\n";
	std::cout << r_reverse << "\n";
	std::cout << should_be_zero << "\n";
	std::cout << s << "\n";
	std::cout << t << "\n";
	std::cout << u << "\n";
	std::cout << v << "\n";
	std::cout << should_also_be_zero << "\n";
	auto p_diff_2 = p.differentiate(2);
	std::cout << "  p: " << p << "\n";
	p.to_derivative();
	std::cout << " p': " << p << "\n";
	p.to_derivative();
	std::cout << "p'': " << p << "\n";
	std::cout << "p'': " << p_diff_2 << "\n";
	auto q_diff_5 = q.differentiate(5);
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	std::cout << q_diff_5 << "\n";
}

void test_gf5_polynomials() {
	Polynomial<GF1<5>> p{{1, 4, 5, 3}};
	Polynomial<GF1<5>> q{{12, 32, 4, 1, 3, 3, 8, 9}};
	auto r_forward = p * q;
	auto r_reverse = q * p;
	auto should_be_zero = r_forward - r_reverse;
	auto s = p + q;
	auto t = p - q;
	auto u = s * t;
	auto v = p * p - q * q;
	auto should_also_be_zero = u - v;
	std::cout << p << "\n";
	std::cout << q << "\n";
	std::cout << r_forward << "\n";
	std::cout << r_reverse << "\n";
	std::cout << should_be_zero << "\n";
	std::cout << s << "\n";
	std::cout << t << "\n";
	std::cout << u << "\n";
	std::cout << v << "\n";
	std::cout << should_also_be_zero << "\n";
	auto p_diff_2 = p.differentiate(2);
	std::cout << "  p: " << p << "\n";
	p.to_derivative();
	std::cout << " p': " << p << "\n";
	p.to_derivative();
	std::cout << "p'': " << p << "\n";
	std::cout << "p'': " << p_diff_2 << "\n";
	auto q_diff_5 = q.differentiate(5);
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	q.to_derivative();
	std::cout << q << "\n";
	std::cout << q_diff_5 << "\n";
}

int main() {
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	test_floating_point_polynomials();
	test_gf5_polynomials();
	return 0;
}
