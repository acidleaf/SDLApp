#ifndef __RANDOM_H__
#define __RANDOM_H__


// Wrapper for the C++11 random generators


#include <random>

class Random {
	std::mt19937 _rng;
public:
	Random() { std::random_device rd; _rng.seed(rd()); }
	Random(unsigned int seed) { _rng.seed(seed); }
	
	float operator()(float max = 1.0f) {
		std::uniform_real_distribution<float> dist(0.0f, max);
		return dist(_rng);
	}
	float operator()(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(_rng);
	}
	
	int operator()(int max = std::mt19937::max()) {
		std::uniform_int_distribution<int> dist(0, max);
		return dist(_rng);
	}
	int operator()(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(_rng);
	}
};

#endif