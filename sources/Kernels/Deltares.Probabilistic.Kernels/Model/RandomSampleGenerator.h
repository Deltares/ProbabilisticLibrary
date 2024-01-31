#pragma once
#include "RandomSettings.h"
#include "Sample.h"
#include "../Reliability/StochastSettingsSet.h"

class RandomSampleGenerator
{
public:
	std::shared_ptr<RandomSettings> Settings = std::make_shared<::RandomSettings>();
	std::shared_ptr<Deltares::Reliability::StochastSettingsSet> StochastSet = std::make_shared<Deltares::Reliability::StochastSettingsSet>();

	void initialize();
	void restart();
	std::shared_ptr<Sample> getRandomSample();
};
