#pragma once
#include <type_traits>

namespace cherry {

template<typename T, std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 0;
}

template<typename T, std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 1;
}

template<typename T, std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
constexpr T inv(const T& val) {
	T one_v = one(&val);
	return one_v / val;
}

template<typename T, std::enable_if<std::is_compound<T>::value>::type* = nullptr>
T inv(const T& a) {
	return a.inv();
}

class GF1_Base {};

template<typename T, std::enable_if_t<std::is_base_of_v<GF1_Base, T>, bool> = true>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 0;
}

template<typename T, std::enable_if_t<std::is_base_of_v<GF1_Base, T>, bool> = true>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	(void)other_data;
	return 1;
}

class GF_Base {};

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Base, T>, bool> = true>
constexpr T zero(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	return zero(T::literally_just_to_store_type_data(), other_data);
}

template<typename T, std::enable_if_t<std::is_base_of_v<GF_Base, T>, bool> = true>
constexpr T one(const T * literally_just_type_info = nullptr, const void * other_data = nullptr) {
	(void)literally_just_type_info;
	return one(T::literally_just_to_store_type_data(), other_data);
}

}
