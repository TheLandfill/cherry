#pragma once
#include "abelian-group.hpp"

namespace cherry {

template<typename T>
class Ring : public Abelian_Group<T> {
public:
	virtual T operator*(const T& other) const = 0;
	virtual T pow_u(unsigned int pow) const = 0;
	virtual T& operator*=(const T& other) = 0;
	virtual ~Ring() = default;
};

template<typename T, std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 1;
}

}
