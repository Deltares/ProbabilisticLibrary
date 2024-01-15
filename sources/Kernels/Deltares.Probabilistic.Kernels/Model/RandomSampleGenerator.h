#pragma once
#include "RandomSettings.h"
#include "Sample.h"

struct XSetting
{
	double xMin = 0;
	double xMax = 1;
	bool isDefault = true;
};

class RandomSampleGenerator
{
private:
	XSetting** xSettings = nullptr;
public:
	RandomSettings* Settings = new ::RandomSettings();

	void initialize();
	void restart();
	Sample* getRandomSample();
};
