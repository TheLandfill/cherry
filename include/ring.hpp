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

}
