#pragma once
#include "RandomSettings.h"
#include "Sample.h"
#include "../Reliability/StochastSettingsSet.h"

class RandomSampleGenerator
{
public:
	RandomSettings* Settings = new ::RandomSettings();
	Deltares::Reliability::StochastSettingsSet* StochastSet = new Deltares::Reliability::StochastSettingsSet();

	void initialize();
	void restart();
	Sample* getRandomSample();
};
