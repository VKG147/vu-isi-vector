#include "average.h"
#include <algorithm>

double getMedian(vector<int> v)
{
	std::sort(v.begin(), v.end());
	if (v.size() % 2 == 1)
		return 1.0 * v.at(v.size() / 2);
	else
		return ((double)v.at((int)(v.size() / 2) - 1) + (double)v.at((int)(v.size() / 2))) / 2;
}

double getMean(vector<int> v)
{
	float avg = 0;

	for (auto it = v.begin(); it != v.end(); ++it)
		avg += *it;

	avg /= v.size();

	return avg;
}