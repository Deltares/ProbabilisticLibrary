#include "LinearRootFinder.h"
#include "../NumericSupport.h"

class XValue
{
public:
	XValue(double x, double value)
	{
		this->X = x;
		this->Value = value;
	}

	double X = 0.0;
	double Value = 0.0;
};

double LinearRootFinder::CalculateValue(double xLow, double xHigh, double target, double tolerance, int maxIterations, RootFinderMethod function, double knownLowValue, double knownHighValue)
{
	constexpr double windowLimit = 0.05;
	constexpr double solutionLimit = 0.2;

	if (xLow > xHigh)
	{
		NumericSupport::swap(xLow, xHigh);
		NumericSupport::swap(knownLowValue, knownHighValue);
	}

	double lowValue = std::isnan(knownLowValue) ? function(xLow) : knownLowValue;
	//Console.WriteLine("f({0:G5}) = {1:G5}", minStart, minResult);

	double highValue = std::isnan(knownHighValue) ? function(xHigh) : knownHighValue;
	//Console.WriteLine("f({0:G5}) = {1:G5}", maxStart, maxResult);

	// Initialize linear search method
	auto low = XValue(xLow,  lowValue);
	auto high = XValue (xHigh,highValue);

	bool ascending = low.Value < high.Value;
	bool descending = low.Value > high.Value;

	auto solution = interpolate(low, high, target, function);

	// Linear search method method
	int iterations = 0;
	double previousWindow = std::numeric_limits<double>::max();
	double previousSolution = std::numeric_limits<double>::max();
	bool bisect = false;

	while (std::abs(solution.Value - target) > tolerance && iterations < maxIterations)
	{
		if (solution.X < low.X)
		{
			high = low;
			low = solution;
		}
		else if (solution.X > high.X)
		{
			low = high;
			high = solution;
		}
		else if (solution.Value < target && high.Value < target)
		{
			high = solution;
		}
		else if (solution.Value > target && high.Value > target)
		{
			high = solution;
		}
		else if (solution.Value < target && high.Value > target)
		{
			low = solution;
		}
		else if (solution.Value > target && high.Value < target)
		{
			low = solution;
		}

		ascending |= low.Value < high.Value;
		descending |= low.Value > high.Value;

		if (ascending && descending)
		{
			return double::NaN;
		}

		// if both the the solution window end the solution do not become significantly smaller, switch over to bisection

		double window = high.X - low.X;
		bisect |= std::abs((solution.Value - target) / (previousSolution - target)) > solutionLimit && window / previousWindow > 1 - windowLimit;

		previousSolution = solution.Value;

		if (bisect)
		{
			solution = bisection(low, high, function);
		}
		else
		{
			solution = interpolate(low, high, target, function);
		}

		if (low.X < solution.X && high.X > solution.X && std::abs(high.X - low.X) < tolerance)
		{
			target = solution.Value;
		}

		previousWindow = window;

		iterations++;
	}

	if (abs(solution.Value - target) > tolerance)
	{
		return double::NaN;
	}

	return solution.X;
}

XValue LinearRootFinder::interpolate(const XValue& low, const XValue& high, double target, RootFinderMethod function)
{
	double x = low.X + ((low.Value - target) / (low.Value - high.Value)) * (high.X - low.X);
	double xValue = function(x);

	return XValue(x, xValue);
}

XValue LinearRootFinder::bisection(const XValue& low, const XValue& high, RootFinderMethod function)
{
	double x = (low.X + high.X) / 2;
	double xValue = function(x);

	return XValue(x, xValue);
}


