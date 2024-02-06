#pragma once
#include <functional>
#include <cmath>

typedef std::function<double(double)> RootFinderMethod;

class RootFinder
{

public:
	virtual double CalculateValue(double xLow, double xHigh, double target, double tolerance, int maxIterations, RootFinderMethod function, double knownLowValue = nan(""), double knownHighValue = nan(""))
	{
		return nan("");
	}
};




