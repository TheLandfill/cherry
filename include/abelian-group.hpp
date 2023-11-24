#pragma once
#include <type_traits>

namespace cherry {

template<typename T>
class Abelian_Group {
public:
	virtual T operator+(const T& other) const = 0;
	virtual T operator-() const = 0;
	virtual void negate() = 0;
	virtual T operator-(const T& other) const = 0;
	virtual T& operator+=(const T& other) = 0;
	virtual T& operator-=(const T& other) = 0;
	virtual ~Abelian_Group() = default;
};

template<typename T, std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 0;
}

}
