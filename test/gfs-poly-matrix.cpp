#include "../include/gf.hpp"
#include "simple-timer.hpp"

using namespace cherry;

#define NAME_AND_VALUE(X) \
std::cout << #X "\n" << X;

Matrix<GF<5>> return_matrix_from_function_test() {
	std::shared_ptr<Polynomial<GF1<5>>> irreducible_poly = std::make_shared<Polynomial<GF1<5>>>(std::vector<GF1<5>>{1, 4, 3, 1});
	GF<5> z = zero<GF<5>>(irreducible_poly.get());
	GF<5> o = one<GF<5>>(irreducible_poly.get());
	GF<5> a{*irreducible_poly.get(), {{3, 2, 1}}};
	GF<5> b{*irreducible_poly.get(), {{3, 1, 4}}};
	GF<5> c{*irreducible_poly.get(), {{0, 2, 1}}};
	Matrix<GF<5>> mat_z{z, 3, irreducible_poly};
	Matrix<GF<5>> mat_o{o, 3, irreducible_poly};
	Matrix<GF<5>> mat_a{a, 3, irreducible_poly};
	Matrix<GF<5>> mat_b{b, 3, irreducible_poly};
	Matrix<GF<5>> mat_c{c, 3, irreducible_poly};

	auto id{ mat_z };
	id.to_identity();
	auto circ{ mat_z };
	circ.to_circulant_diag(1);
	circ *= c;
	circ += b * (circ^2);
	circ += a * id;
	auto nil1{ mat_z };
	nil1.to_nilpotent_diag(1, a);
	auto nil2{ mat_z };
	nil2.to_nilpotent_diag(-1, c);
	auto mat = mat_o + (mat_o^0) + (mat_o^2) + ((mat_a + mat_b)^2) + ((mat_a * mat_c + mat_b)^4) + id + circ + nil1 + nil2;

	NAME_AND_VALUE(mat_z)
	NAME_AND_VALUE(mat_o)
	NAME_AND_VALUE(mat_a)
	NAME_AND_VALUE(mat_b)
	NAME_AND_VALUE(mat_c)
	NAME_AND_VALUE(id)
	NAME_AND_VALUE(circ)
	NAME_AND_VALUE(nil1)
	NAME_AND_VALUE(nil2)
	NAME_AND_VALUE(mat)

	std::cout << "Matrix powers\n";
	for (size_t i = 0; i < 10; i++) {
		NAME_AND_VALUE((mat^i))
	}
	std::cout << "Number of objects using poly: " << irreducible_poly.use_count() << "\n";
	std::cout << mat.to_string();
	return mat;
}

int main() {
	Timer t(__FILE__);
	std::cout << "--------------------------------------------------------------------------------\n";
	std::cout << __FILE__ << "\n";
	std::cout << "--------------------------------------------------------------------------------\n";
	auto mat = return_matrix_from_function_test();
	std::cout << "Matrix returned from test!\n";
	std::cout << mat;
	std::cout << mat.use_count() << "\n";
	std::cout << "Inverse\n";
	std::cout << mat.inv();
	std::cout << "Product\n";
	std::cout << mat * mat.inv();
	std::cout << mat.inv() * mat;
}
