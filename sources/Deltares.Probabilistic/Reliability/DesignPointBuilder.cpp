// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "../Model/Sample.h"
#include "../Math/NumericSupport.h"
#include "../Model/ModelRunner.h"
#include "DesignPointBuilder.h"

#include <cmath>
#include <map>
#include <stdexcept>

namespace Deltares::Reliability
{
    class ModeFinder
    {
    private:
        std::map<double, double> values;
        std::shared_ptr<Statistics::Stochast> stochast = nullptr;

    public:
        explicit ModeFinder(const std::shared_ptr<Statistics::Stochast>& stochast) : stochast(stochast)
        {
        }

        void add(double u, double weight)
        {
            u = stochast->getRepresentativeU(u);

            if (!values.contains(u))
            {
                values[u] = 0;
            }

            values[u] += weight;
        }

        void clear()
        {
            values.clear();
        }

        double getMode() const
        {
            double mode = 0;
            double max = -1;

            for (const auto& [u, weight] : values)
            {
                if (weight > max)
                {
                    mode = u;
                    max = weight;
                }
            }

            return mode;
        }
    };

    DesignPointBuilder::DesignPointBuilder(int count, DesignPointMethod method,
        const std::shared_ptr<StochastSettingsSet>& stochastSet, bool addProbability)
        : count(count), method(method), addProbability(addProbability)
    {
        if (addProbability)
        {
            this->count += 1;
        }

        initializeTotals();

        if (stochastSet != nullptr)
        {
            for (int i = 0; i < stochastSet->getVaryingStochastCount(); i++)
            {
                if (stochastSet->VaryingStochastSettings[i]->IsQualitative)
                {
                    qualitativeIndices.push_back(i);
                    modeFinders.push_back(std::make_shared<ModeFinder>(stochastSet->VaryingStochastSettings[i]->stochast));
                }
            }
        }

        qualitativeCount = static_cast<int>(qualitativeIndices.size());
    }

    DesignPointBuilder::DesignPointBuilder(DesignPointMethod method,
        const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts)
        : count(static_cast<int>(stochasts.size())), method(method)
    {
        initializeTotals();

        for (int i = 0; i < count; i++)
        {
            if (stochasts[i]->isQualitative())
            {
                qualitativeIndices.push_back(i);
                modeFinders.push_back(std::make_shared<ModeFinder>(stochasts[i]));
            }
        }

        qualitativeCount = static_cast<int>(qualitativeIndices.size());
    }

    void DesignPointBuilder::initializeTotals()
    {
        defaultSample = Models::Sample(count, addProbability);
        meanSample = Models::Sample(count, addProbability);
        sinSample = Models::Sample(count, addProbability);
        cosSample = Models::Sample(count, addProbability);

        for (const auto& modeFinder : modeFinders)
        {
            modeFinder->clear();
        }

        sumWeights = 0;
    }

    void DesignPointBuilder::initialize(double beta) 
    {
        double value = Numeric::NumericSupport::GetSign(beta) * sqrt(std::abs(beta) / count);

        for (int i = 0; i < count; i++)
        {
            defaultSample.Values[i] += value;
        }
    }

    void DesignPointBuilder::addSample(Models::Sample& modelSample, double probability)
    {
        sampleAdded = true;

        Models::Sample sample = modelSample;
        if (addProbability)
        {
            sample = getSampleWithProbability(modelSample, probability);
        }

        double weight = std::isnan(sample.Weight) ? 1.0 : sample.Weight;

        if (!weightedSampleAdded && method != DesignPointMethod::NearestToMean)
        {
            // assign a preliminary weight when weight is zero, restart counting when a real weight arrives
            if (weight > 0)
            {
                this->initializeTotals();
                weightedSampleAdded = true;
            }
            else
            {
                weight = 1.0;
            }
        }

        weight *= probability;

        handleSample(sample, weight);
    }

    void DesignPointBuilder::removeSample(Models::Sample& sample)
    {
        double weight = std::isnan(sample.Weight) ? -1 : -sample.Weight;

        handleSample(sample, weight);
    }

    void DesignPointBuilder::handleSample(Models::Sample& sample, double weight)
    {
        constexpr double delta = 1E-10;

        switch (method)
        {
        case DesignPointMethod::NearestToMean:
        {
            if (weight < 0)
            {
                if (sampleAdded)
                {
                    bool updateMinimumSample = nearestSamples.back().areValuesEqual(sample);

                    std::erase_if(nearestSamples, [&sample](auto s) { return sample.areValuesEqual(s); });
                    
                    sampleAdded = !nearestSamples.empty();

                    if (updateMinimumSample)
                    {
                        if (sampleAdded)
                        {
                            minimumBeta = nearestSamples.back().getBeta();
                            meanSample = nearestSamples.back().clone();
                        }
                        else
                        {
                            minimumBeta = std::numeric_limits<double>::infinity();
                            meanSample = Models::Sample();
                        }
                    }
                }
            }
            else
            {
                double beta = sample.getBeta();

                if (beta < minimumBeta)
                {
                    if (Numeric::NumericSupport::areEqual(beta, Statistics::StandardNormal::BetaMax, delta))
                    {
                        minimumBeta = beta - 2.0 * delta;
                    }
                    else
                    {
                        minimumBeta = beta;
                    }

                    nearestSamples.push_back(sample);

                    meanSample = sample.clone();
                }
            }
            break;
        }
        case DesignPointMethod::CenterOfGravity:
        {
            for (int j = 0; j < this->qualitativeCount; j++)
            {
                int qIndex = qualitativeIndices[j];
                modeFinders[j]->add(sample.Values[qIndex], weight);
            }

            for (int i = 0; i < sample.getSize(); i++)
            {
                meanSample.Values[i] += weight * sample.Values[i];
            }

            sumWeights += weight;
            break;
        }
        case DesignPointMethod::CenterOfAngles:
        {
            for (int j = 0; j < this->qualitativeCount; j++)
            {
                int qIndex = qualitativeIndices[j];
                modeFinders[j]->add(sample.Values[qIndex], weight);
            }

            auto sphericalValues = Numeric::NumericSupport::GetSphericalCoordinates(sample.Values);
            meanSample.Values[0] += weight * sphericalValues[0];
            for (int i = 1; i < count; i++)
            {
                sinSample.Values[i] += weight * std::sin(sphericalValues[i]);
                cosSample.Values[i] += weight * std::cos(sphericalValues[i]);
            }
            sumWeights += weight;
            break;
        }
        default:
            throw std::runtime_error("Not supported");
        }
    }

    Models::Sample DesignPointBuilder::getSample() const
    {
        if (!sampleAdded)
        {
            return defaultSample.clone();
        }
        else
        {
            switch (method)
            {
            case DesignPointMethod::NearestToMean:
            {
                return meanSample.clone();
            }
            case DesignPointMethod::CenterOfGravity:
            {
                Models::Sample gravityPoint = Models::Sample(count, addProbability);

                for (int i = 0; i < count; i++)
                {
                    gravityPoint.Values[i] = meanSample.Values[i] / sumWeights;
                }

                for (int j = 0; j < this->qualitativeCount; j++)
                {
                    int qIndex = qualitativeIndices[j];
                    gravityPoint.Values[qIndex] = modeFinders[j]->getMode();
                }

                return gravityPoint;
            }
            case DesignPointMethod::CenterOfAngles:
            {
                auto angleValues = std::vector<double>(count);
                angleValues[0] = meanSample.Values[0] / sumWeights;
                for (int i = 1; i < count; i++)
                {
                    angleValues[i] = std::atan2(sinSample.Values[i] / sumWeights, cosSample.Values[i] / sumWeights);
                }

                auto coordinates = Numeric::NumericSupport::GetCartesianCoordinates(angleValues);
                Models::Sample anglePoint = Models::Sample(coordinates, addProbability);

                for (int j = 0; j < this->qualitativeCount; j++)
                {
                    int qIndex = qualitativeIndices[j];
                    anglePoint.Values[qIndex] = modeFinders[j]->getMode();
                }

                return anglePoint;
            }
            default:
                throw std::runtime_error("Not supported");
            }
        }
    }

    Models::Sample DesignPointBuilder::getSampleWithProbability(Models::Sample& sample, double probability)
    {
        double pAveraged = (1 + (1 - probability)) / 2;
        double uAveraged = Statistics::StandardNormal::getUFromP(pAveraged);

        auto uCopy = sample.getExtendedSample(uAveraged);

        if (std::isnan(uCopy.Weight))
        {
            uCopy.Weight = probability;
        }
        else
        {
            uCopy.Weight *= probability;
        }

        return uCopy;
    }

    std::string DesignPointBuilder::getDesignPointMethodString(DesignPointMethod method)
    {
        switch (method)
        {
        case DesignPointMethod::NearestToMean: return "nearest_to_mean";
        case DesignPointMethod::CenterOfGravity: return "center_of_gravity";
        case DesignPointMethod::CenterOfAngles: return "center_of_angles";
        default: throw ProbabilisticLibraryException("Design point method");
        }
    }

    DesignPointMethod DesignPointBuilder::getDesignPointMethod(const std::string& method)
    {
        if (method == "nearest_to_mean") return DesignPointMethod::NearestToMean;
        else if (method == "center_of_gravity") return DesignPointMethod::CenterOfGravity;
        else if (method == "center_of_angles") return DesignPointMethod::CenterOfAngles;
        else throw ProbabilisticLibraryException("Design point method");
    }
}

