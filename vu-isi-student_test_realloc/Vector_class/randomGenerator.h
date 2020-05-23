#include <random>

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

class RandomGenerator
{
private:
	std::mt19937 generator;
	
public:
	RandomGenerator();
	RandomGenerator(unsigned int seed);
	
	int getRandom(int a, int b);
	double getRandom(double a, double b);
};

#endif