#pragma once
#include <limits>
#include <vector>

#include "Sample.h"

typedef double(__stdcall* ZDelegate) (Sample*);

typedef double(__stdcall* ZMultipleDelegate) (Sample**, int count);

double inline getDefaultZDelegate(Sample*)
{
	return std::numeric_limits<double>::quiet_NaN();
}

class ZModel
{
private:
	ZDelegate zDelegate;
	ZMultipleDelegate zMultipleDelegate;
	bool zMultipleDelegateAssigned = false;

public:
	ZModel()
	{
		this->zDelegate = &getDefaultZDelegate;
	}

	void setZDelegate(ZDelegate zDelegate)
	{
		this->zDelegate = zDelegate;
	}

	void setZMultipleDelegate(ZMultipleDelegate zMultipleDelegate)
	{
		this->zMultipleDelegate = zMultipleDelegate;
		zMultipleDelegateAssigned = true;
	}

	void invoke(Sample* sample)
	{
		this->zDelegate(sample);
	}

	void invoke(std::vector<Sample*> samples)
	{
		if (!zMultipleDelegateAssigned) 
		{
			for (int i = 0; i < samples.size(); i++)
			{
				invoke(samples[i]);
			}
		}
		else
		{
			Sample** sampleList = new Sample*[samples.size()];

			for (int i = 0; i < samples.size(); i++)
			{
				sampleList[i] = samples[i];
			}

			this->zMultipleDelegate(sampleList, samples.size());
		}
	}
};

