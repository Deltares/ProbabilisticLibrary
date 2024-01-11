#pragma once
#include <unordered_map>

#include "../Model/Sample.h"
#include "../Model/ZModelRunner.h"

enum DesignPointMethod
{
    CenterOfGravity,
    CenterOfAngles,
    NearestToMean
};

class DesignPointBuilder
{
private:
    int count = 0;
    DesignPointMethod method = DesignPointMethod::NearestToMean;
    double rmin = std::numeric_limits<double>::infinity();
    double sumWeights = 0;
    Sample* defaultSample = nullptr;
    Sample* meanSample = nullptr;
    Sample* sinSample = nullptr;
    Sample* cosSample = nullptr;
    bool sampleAdded = false;
    Deltares::Models::ZModelRunner* model = nullptr;
    std::unordered_map<int, double> scenarioWeights;

    void RegisterScenario(Sample* sample, double weight = 1);
    int GetMaxScenario();

public:
    DesignPointBuilder(int count, DesignPointMethod method, Deltares::Models::ZModelRunner* model = nullptr);
    void Initialize(double beta);
    void AddSample(Sample* sample, double weight = 1);
    Sample* GetSample();
};





