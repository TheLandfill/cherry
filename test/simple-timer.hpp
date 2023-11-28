#pragma once
#include <chrono>
#include <iostream>

class Timer {
public:
	Timer(const char * func_name) : name{func_name}, start_time{std::chrono::high_resolution_clock::now()} {}
	~Timer() {
		auto end_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = end_time - start_time;
		std::cout << name << " took " << duration.count() << " seconds.\n";
	}
private:
	const char * name;
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
