#pragma once
#include "ring.hpp"

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

}
