#include "randomGenerator.h"

RandomGenerator::RandomGenerator()
{
	std::random_device rd;
	this->generator = std::mt19937(rd());
}

RandomGenerator::RandomGenerator(unsigned int seed)
{
	this->generator = std::mt19937(seed);
}

int RandomGenerator::getRandom(int a, int b)
{
	std::uniform_int_distribution<int> dist(a, b);
	return dist(this->generator);
}

double RandomGenerator::getRandom(double a, double b)
{
	std::uniform_real_distribution<double> dist(a, b);
	return dist(this->generator);
}
