#pragma once
#include <cctype>
#include <type_traits>

namespace cherry {

template<typename T, typename std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T zero(const void * other_data = nullptr) {
	(void)other_data;
	return 0;
}

template<typename T, typename std::enable_if<std::is_fundamental<T>::value>::type* = nullptr>
constexpr T one(const void * other_data = nullptr) {
	(void)other_data;
	return 1;
}

template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
constexpr T inv(const T& val) {
	T one_v = one<T>();
	return one_v / val;
}

template<typename T, typename std::enable_if<std::is_compound<T>::value>::type* = nullptr>
T inv(const T& a) {
	return a.inv();
}

template<typename T, typename std::enable_if<std::is_compound<T>::value>::type* = nullptr>
constexpr T zero(const void * other_data = nullptr) {
	return T::member_zero(other_data);
}

template<typename T, typename std::enable_if<std::is_compound<T>::value>::type* = nullptr>
constexpr T one(const void * other_data = nullptr) {
	return T::member_one(other_data);
}

// class GF1_Base {};
// 
// template<typename T, std::enable_if_t<std::is_base_of_v<GF1_Base, T>, bool> = true>
// constexpr T zero(const void * other_data = nullptr) {
// 	(void)other_data;
// 	return 0;
// }
// 
// template<typename T, std::enable_if_t<std::is_base_of_v<GF1_Base, T>, bool> = true>
// constexpr T one(const void * other_data = nullptr) {
// 	(void)other_data;
// 	return 1;
// }
// 
// class GF_Base {};
// 
// template<typename T, std::enable_if_t<std::is_base_of_v<GF_Base, T>, bool> = true>
// constexpr T zero(const void * other_data) {
// 	return T::gfs_zero(other_data);
// }
// 
// template<typename T, std::enable_if_t<std::is_base_of_v<GF_Base, T>, bool> = true>
// constexpr T one(const void * other_data) {
// 	return T::gfs_one(other_data);
// }

}
