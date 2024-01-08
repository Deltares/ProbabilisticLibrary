#pragma once
#include <limits>

typedef double (*ZDelegate) (double*, int count);

double inline getDefaultZDelegate(double* values, int count)
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

	double invoke(double* values, int count)
	{
		return this->zDelegate(values, count);
	}
};

