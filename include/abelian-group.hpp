#pragma once

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

}
