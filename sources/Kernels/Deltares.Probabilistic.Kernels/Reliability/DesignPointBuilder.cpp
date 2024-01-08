#include "../Model/Sample.h"
#include "../Math/NumericSupport.h"
#include "../Model/ZModelRunner.h"
#include "DesignPointBuilder.h"

#include <vector>
#include <cmath>
#include <unordered_map>
#include <limits>
#include <stdexcept>

//class ModelRunner
//{
//public:
//    int* GetQualitativeValues(Sample sample)
//    {
//        // implementation
//    }
//};

DesignPointBuilder::DesignPointBuilder(int count, DesignPointMethod method, ZModelRunner* model)
{
	this->count = count;
	this->method = method;
	this->model = model;

	defaultSample = new Sample(count);
	meanSample = new Sample(count);
	sinSample = new Sample(count);
	cosSample = new Sample(count);
}

void DesignPointBuilder::Initialize(double beta)
{
	double value = sqrt(beta / count);

	for (int i = 0; i < count; i++)
	{
		defaultSample->Values[i] = value;
	}
}

void DesignPointBuilder::AddSample(Sample* sample, double weight)
{
	sampleAdded = true;
	// set scenario defining parameters to zero
	RegisterScenario(sample, weight);
	switch (method)
	{
	case DesignPointMethod::NearestToMean:
	{
		double rbeta = sample->getBeta();
		if (rbeta < rmin)
		{
			rmin = rbeta;
			meanSample = sample;
		}
		break;
	}
	case DesignPointMethod::CenterOfGravity:
	{
		for (int i = 0; i < sample->getSize(); i++)
		{
			meanSample->Values[i] += weight * sample->Values[i];
		}
		sumWeights += weight;
		break;
	}
	case DesignPointMethod::CenterOfAngles:
	{
		double* sphericalValues = NumericSupport::GetSphericalCoordinates(sample->Values, count);
		meanSample->Values[0] += weight * sphericalValues[0];
		for (int i = 1; i < count; i++)
		{
			sinSample->Values[i] += weight * std::sin(sphericalValues[i]);
			cosSample->Values[i] += weight * std::cos(sphericalValues[i]);
		}
		sumWeights += weight;
		break;
	}
	default:
		throw std::runtime_error("Not supported");
	}
}

Sample* DesignPointBuilder::GetSample()
{
	if (!sampleAdded)
	{
		return defaultSample;
	}
	else
	{
		switch (method)
		{
		case DesignPointMethod::NearestToMean:
		{
			meanSample->ScenarioIndex = GetMaxScenario();
			return meanSample;
		}
		case DesignPointMethod::CenterOfGravity:
		{
			double* gravityValues = new double[count];

			for (int i = 0; i < count; i++)
			{
				gravityValues[i] = meanSample->Values[i] / sumWeights;
			}

			Sample* gravityPoint = new Sample(gravityValues, count);
			gravityPoint->ScenarioIndex = GetMaxScenario();
			return gravityPoint;
		}
		case DesignPointMethod::CenterOfAngles:
		{
			double* angleValues = new double[count];
			angleValues[0] = meanSample->Values[0] / sumWeights;
			for (int i = 1; i < count; i++)
			{
				angleValues[i] = std::atan2(sinSample->Values[i] / sumWeights, cosSample->Values[i] / sumWeights);
			}
			double* coordinates = NumericSupport::GetCartesianCoordinates(angleValues, count);
			Sample* anglePoint = new Sample(coordinates, count);
			anglePoint->ScenarioIndex = GetMaxScenario();
			return anglePoint;
		}
		default:
			throw std::runtime_error("Not supported");
		}
	}
}

void DesignPointBuilder::RegisterScenario(Sample* sample, double weight)
{
	//int* scenarios = model != nullptr ? model->GetQualitativeValues(sample) : new int[0];
	//for (int i = 0; i < scenarios.Length; i++)
	//{
	//    if (scenarioWeights.find(scenarios[i]) == scenarioWeights.end())
	//    {
	//        scenarioWeights[scenarios[i]] = 0;
	//    }
	//    scenarioWeights[scenarios[i]] += weight;
	//}
}

int DesignPointBuilder::GetMaxScenario()
{
	int maxScenario = -1;
	double maxScenarioWeight = -1;
	if (scenarioWeights.size() > 0)
	{
		for (auto it = scenarioWeights.begin(); it != scenarioWeights.end(); ++it)
		{
			if (it->second > maxScenarioWeight)
			{
				maxScenario = it->first;
				maxScenarioWeight = it->second;
			}
		}
		return maxScenario;
	}
	else
	{
		return -1;
	}
}



