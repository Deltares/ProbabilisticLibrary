#pragma once
#include <limits>

typedef double (*ZDelegate) (double*);

double inline getDefaultZDelegate(double* values)
{
	return std::numeric_limits<double>::quiet_NaN();
}

class ZModel
{
private:
	ZDelegate zDelegate;

public:
	ZModel()
	{
		this->zDelegate = &getDefaultZDelegate;
	}

	void setZDelegate(ZDelegate zDelegate)
	{
		this->zDelegate = zDelegate;
	}

	double getZValue(double* values)
	{
		return this->zDelegate(values);
	}
};

