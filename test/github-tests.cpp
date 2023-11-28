#include "../include/gf.hpp"
#include "simple-timer.hpp"
#include <iostream>

using namespace cherry;

void gf1_test() {
	GF1<5> a = 3;
	GF1<5> b = 4;

	auto c = a + b;
	auto d = a - b;
	auto e = c * d;
	auto f = a * a - (b^2);
	auto should_be_zero = e - f;
	auto g = c / d;


	std::cout << "a = " << a << "\n";
	std::cout << "b = " << b << "\n";
	std::cout << "c = " << c << "\n";
	std::cout << "d = " << d << "\n";
	std::cout << "e = " << e << "\n";
	std::cout << "f = " << f << "\n";
	std::cout << "g = " << g << "\n";
	std::cout << "0 = " << should_be_zero << "\n";
}

void gf_general_test() {
	Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};

	Polynomial<GF1<5>> poly1{{3, 2, 1}};
	Polynomial<GF1<5>> poly2{{3, 1, 4}};

	GF<5> a{irreducible_poly, poly1};
	GF<5> b{irreducible_poly, poly2};

	auto c = a + b;
	auto d = a - b;
	auto e = a * b;
	auto f = a / b;
	auto g = c * d;
	auto h = a * a - (b^2);
	auto should_be_zero = g - h;
	auto i = c / d;


	std::cout << "a = " << a << "\n";
	std::cout << "b = " << b << "\n";
	std::cout << "c = " << c << "\n";
	std::cout << "d = " << d << "\n";
	std::cout << "e = " << e << "\n";
	std::cout << "f = " << f << "\n";
	std::cout << "g = " << g << "\n";
	std::cout << "h = " << h << "\n";
	std::cout << "i = " << i << "\n";
	std::cout << "0 = " << should_be_zero << "\n";
}

int main() {
	Timer t(__FILE__);
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	gf1_test();
	gf_general_test();
	return 0;
}

