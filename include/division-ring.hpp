#pragma once
#include "ring.hpp"
#include <type_traits>

namespace cherry {

template<typename T>
class Division_Ring : public Ring<T> {
public:
	virtual T operator/(const T& other) const = 0;
	virtual T operator^(int pow) const = 0;
	virtual T inv() const = 0;
	virtual T& operator/=(const T& other) = 0;
	virtual ~Division_Ring() = default;
};

template<typename T, std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
constexpr T inv(const T& val) {
	T one_v = one(&val);
	return one_v / val;
}

template<typename T, std::enable_if<std::is_compound<T>::value>::type* = nullptr>
T inv(const T& a) {
	return a.inv();
}

}
