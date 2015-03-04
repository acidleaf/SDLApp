#ifndef __ELAPSED_H__
#define __ELAPSED_H__

#include <chrono>

class Elapsed {
protected:
	std::chrono::high_resolution_clock::time_point _begin;
public:
	void begin() { _begin = std::chrono::high_resolution_clock::now(); }
	std::chrono::microseconds end() {
		auto diff = std::chrono::high_resolution_clock::now() - _begin;
		return std::chrono::duration_cast<std::chrono::microseconds>(diff);
	}
};

#endif