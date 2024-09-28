#pragma once
#include <array>
#include <concepts>
#include <cstddef>
#include "identities.hpp"

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

template<typename Input_Type = double>
struct Interval {
	Interval(const Input_Type& s, const Input_Type& e, size_t n) :
		start{s},
		end{e},
		num{n}
	{}
	Input_Type start, end;
	size_t num;
};

// Generalization of a Left, Right, or Midpoint Riemann sum that defaults to a
// midpoint sum of an integral of a function from ℝ ↦ ℝ.
template<
	validation::Valid_Input_Type Input_Type = double,
	typename Scale_Type = Input_Type,
	size_t Num_Dimensions = 1
>
requires validation::Type_And_Scale_Type_Match<Input_Type, Scale_Type>
class Regular_Input_Value_Generator {
public:
	Regular_Input_Value_Generator(
		const std::array<Interval<Input_Type>, Num_Dimensions>& iv,
		Input_Type offset = one_half<Input_Type>(),
		Scale_Type one_v = one<Scale_Type>()
	) :
		intervals{iv},
		scale{one_v}
	{
		std::fill(cur_iteration.begin(), cur_iteration.end(), 0);
		for (size_t i = 0; i < intervals.size(); i++) {
			cur_delta_input[i] = (intervals[i].end - intervals[i].start) / intervals[i].num;
			intervals[i].start += cur_delta_input[i] * offset;
			intervals[i].end += cur_delta_input[i] * offset;
			cur_value[i] = intervals[i].start;
			scale *= cur_delta_input[i];
		}
	}

	Input_Type cur_input() const {
		return cur_value;
	}

	void next_input() {
		for (size_t i = 0; i < Num_Dimensions; i++) {
			cur_iteration[i]++;
			if (cur_iteration[i] != intervals[i].num) {
				cur_value[i] = intervals[i].start + cur_delta_input[i] * cur_iteration[i];
				return;
			}
			cur_iteration[i] = 0;
			cur_value[i] = intervals[i].start;
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
		std::fill(cur_iteration.begin(), cur_iteration.end(), 0);
		for (size_t i = 0; i < intervals.size(); i++) {
			cur_delta_input[i] = (intervals[i].end - intervals[i].start) / intervals[i].num;
			cur_value[i] = intervals[i].start;
		}
	}
private:
	std::array<Interval<Input_Type>, Num_Dimensions> intervals;
	std::array<size_t, Num_Dimensions> cur_iteration;
	std::array<Input_Type, Num_Dimensions> cur_value;
	std::array<Input_Type, Num_Dimensions> cur_delta_input;
	Scale_Type scale;
	bool not_all_inputs_used = true;
};

template<
	typename Func,
	validation::Valid_Input_Type Input_Type = double,
	typename Output_Type = Input_Type,
	typename Scale_Type = Output_Type,
	size_t Num_Dimensions = 1,
	typename Input_Value_Generator = Regular_Input_Value_Generator<
		Input_Type,
		Scale_Type,
		Num_Dimensions
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
			sum += f(input_value_generator.cur_input());
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
