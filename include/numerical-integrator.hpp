#pragma once
#include <array>
#include <concepts>
#include <cstddef>
#ifdef DEBUG
#include <iostream>
#include <format>
#endif
#include "identities.hpp"
#include "vector.hpp"

namespace cherry::numerics {

namespace validation {

template<typename Input_Type>
concept Valid_Input_Type = requires(Input_Type a, size_t c) {
	a += a;
	a - a;
	a *= c;
	a * c;
	a / c;
};

template<typename Type, typename Scale_Type>
concept Type_And_Scale_Type_Match = requires(Type t, Scale_Type s) {
	t *= s;
};

template<typename Output_Type, typename Scale_Type>
concept Output_And_Scale_Type_Match = requires(Output_Type ot, Scale_Type s) {
	ot *= s;
};

template<typename Input_Type, typename Scale_Type, typename Input_Value_Generator>
concept Valid_Input_Value_Generator = requires(Input_Value_Generator ivg) {
	{ ivg.has_inputs() } -> std::same_as<bool>;
	{ ivg.has_sub_inputs() } -> std::same_as<bool>;
	{ ivg.cur_input() } -> std::convertible_to<const Input_Type&>;
	{ ivg.next_input() } -> std::same_as<void>;
	{ ivg.cur_scale() } -> std::convertible_to<const Scale_Type&>;
	{ ivg.next_scale() } -> std::same_as<void>;
	{ ivg.reset() } -> std::same_as<void>;
};

template<typename Input_Type, typename Output_Type, typename Func_Type>
concept Valid_Function_Type = requires(Func_Type f, Input_Type i) {
	{ f(i) } -> std::same_as<Output_Type>;
};

}

template<typename T>
constexpr T one_half(const void * other_data = nullptr) {
	T one_v = one<T>(other_data);
	return one_v / (one_v + one_v);
}

template<typename Input_Type>
struct Interval {
	Interval(const Input_Type& s, const Input_Type& e) :
		start{s},
		end{e}
	{}
	Input_Type start, end;
};

// Generalization of a Left, Right, or Midpoint Riemann sum that defaults to a
// midpoint sum of an integral of a function from ℝ ↦ ℝ.
template<
	size_t Num_Dimensions = 1,
	validation::Valid_Input_Type Input_Type = Vector<Num_Dimensions, double>,
	typename Scale_Type = double
>
requires validation::Type_And_Scale_Type_Match<Input_Type, Scale_Type>
class Regular_Input_Value_Generator {
public:
	Regular_Input_Value_Generator(
		const Interval<Input_Type>& iv,
		const Vector<Num_Dimensions, size_t>& ns,
		Scale_Type offset = one_half<Scale_Type>(),
		Scale_Type one_v = one<Scale_Type>()
	) :
		intervals{iv},
		num_splits{ns},
		scale{one_v}
	{
		cur_iteration.to_zero_vector();
		cur_delta_input = intervals.end - intervals.start;
		for (size_t i = 0; i < Num_Dimensions; i++) {
			cur_delta_input[i] /= num_splits[i];
		}
		intervals.start += cur_delta_input * offset;
		intervals.end += cur_delta_input * offset;
		for (size_t i = 0; i < Num_Dimensions; i++) {
			scale *= cur_delta_input[i];
		}
		cur_value = intervals.start;
	}

	const Input_Type& cur_input() const {
		return cur_value;
	}

	void next_input() {
		for (size_t i = 0; i < Num_Dimensions; i++) {
			cur_iteration[i]++;
			if (cur_iteration[i] != num_splits[i]) {
				cur_value[i] = intervals.start[i] + cur_delta_input[i] * cur_iteration[i];
				return;
			}
			cur_iteration[i] = 0;
			cur_value[i] = intervals.start[i];
		}
		not_all_inputs_used = false;
	}

	void next_scale() {}

	bool has_inputs() const {
		return not_all_inputs_used;
	}

	bool has_sub_inputs() const {
		return has_inputs();
	}

	Scale_Type cur_scale() const {
		return scale;
	}

	void reset() {
		cur_iteration.to_zero_vector();
		cur_delta_input = intervals.end - intervals.start;
		for (size_t i = 0; i < Num_Dimensions; i++) {
			cur_delta_input[i] /= num_splits[i];
		}
		cur_value = intervals.start;
	}
private:
	Interval<Input_Type> intervals;
	Vector<Num_Dimensions, size_t> num_splits;
	Vector<Num_Dimensions, size_t> cur_iteration;
	Input_Type cur_value;
	Input_Type cur_delta_input;
	Scale_Type scale;
	bool not_all_inputs_used = true;
};

template<
	validation::Valid_Input_Type Input_Type = Vector<1, double>,
	typename Scale_Type = double
>
requires validation::Type_And_Scale_Type_Match<Input_Type, Scale_Type>
class Trapezoid_Rule_Value_Generator {
public:
	Trapezoid_Rule_Value_Generator(
		const Interval<Input_Type>& iv,
		const Vector<1, size_t>& num_divisions_not_counting_endpoints,
		Scale_Type one_half_v = one_half<Scale_Type>()
	) :
		intervals{iv},
		num_splits{num_divisions_not_counting_endpoints},
		scale{one_half_v}
	{
		cur_iteration.to_zero_vector();
		cur_delta_input = intervals.end - intervals.start;
		cur_delta_input[0] /= num_splits[0];
		scale *= cur_delta_input[0];
		cur_value = intervals.start;
	}

	const Input_Type& cur_input() const {
		return cur_value;
	}

	void next_input() {
		cur_iteration[0]++;
		if (stage == 0) [[unlikely]] {
			cur_value[0] = intervals.end[0];
			if (cur_iteration[0] == 1) {
				return;
			} else {
				cur_iteration[0] = 1;
				stage++;
				not_all_inputs_used = false;
				cur_value[0] = intervals.start[0] + cur_delta_input[0];
			}
		}
		if (cur_iteration[0] < num_splits[0]) {
			cur_value[0] = intervals.start[0] + cur_delta_input[0] * cur_iteration[0];
			return;
		}
		stage++;
		not_all_inputs_used = false;
	}

	void next_scale() {
		scale *= 2.0;
		not_all_inputs_used = cur_iteration[0] < num_splits[0];
	}

	bool has_inputs() const {
		return stage < 2;
	}

	bool has_sub_inputs() const {
		return not_all_inputs_used;
	}

	Scale_Type cur_scale() const {
		return scale;
	}

	size_t get_num_splits() const {
		return num_splits[0];
	}

	void reset() {
		cur_iteration.to_zero_vector();
		cur_delta_input = intervals.end - intervals.start;
		if (num_splits[0] > 0) {
			cur_delta_input[0] /= num_splits[0];
		}
		cur_value = intervals.start;
	}
private:
	Interval<Input_Type> intervals;
	Vector<1, size_t> num_splits;
	Vector<1, size_t> cur_iteration;
	Input_Type cur_value;
	Input_Type cur_delta_input;
	Scale_Type scale;
	size_t stage = 0;
	bool not_all_inputs_used = true;
};

template<
	typename Func,
	size_t Num_Dimensions = 1,
	validation::Valid_Input_Type Input_Type = Vector<Num_Dimensions, double>,
	typename Output_Type = Input_Type,
	typename Scale_Type = double,
	typename Input_Value_Generator = Regular_Input_Value_Generator<
		Num_Dimensions,
		Input_Type,
		Scale_Type
	>
>
requires
	validation::Type_And_Scale_Type_Match<Output_Type, Scale_Type>
		&& validation::Valid_Input_Value_Generator<Input_Type, Scale_Type, Input_Value_Generator>
		&& validation::Valid_Function_Type<Input_Type, Output_Type, Func>
Output_Type n_integrate(Input_Value_Generator& input_value_generator, Func f) {
	Output_Type output{};
	while (input_value_generator.has_inputs()) {
		Output_Type sum{};
		while (input_value_generator.has_sub_inputs()) {
			const Input_Type& in = input_value_generator.cur_input();
			sum += f(in);
			#ifdef DEBUG
			std::cout << std::format("({:7}, {:2.3}, {:2.3})\n", input_value_generator.get_num_splits(), in[0], input_value_generator.cur_scale());
			#endif
			input_value_generator.next_input();
		}
		sum *= input_value_generator.cur_scale();
		input_value_generator.next_scale();
		output += sum;
	}
	input_value_generator.reset();
	return output;
}

}
