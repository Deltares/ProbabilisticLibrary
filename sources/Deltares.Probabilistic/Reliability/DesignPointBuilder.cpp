// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
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

#include <vector>
#include <cmath>
#include <map>
#include <stdexcept>

namespace Deltares
{
    namespace Reliability
    {
        class ModeFinder
        {
        private:
            std::map<double, double> values;
            std::shared_ptr<Statistics::Stochast> stochast = nullptr;

        public:
            ModeFinder(std::shared_ptr<Statistics::Stochast> stochast)
            {
                this->stochast = stochast;
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

            double getMode()
            {
                double mode = 0;
                double max = -1;

                for (auto it = values.begin(); it != values.end(); ++it)
                {
                    if (it->second > max)
                    {
                        mode = it->first;
                        max = it->second;
                    }
                }

                return mode;
            }
        };

        DesignPointBuilder::DesignPointBuilder(int count, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet)
        {
            initializeSamples(count, method);

            if (stochastSet != nullptr)
            {
                for (int i = 0; i < stochastSet->getVaryingStochastCount(); i++)
                {
                    if (stochastSet->VaryingStochastSettings[i]->IsQualitative)
                    {
                        this->qualitativeIndices.push_back(i);
                        this->modeFinders.push_back(std::make_shared<ModeFinder>(stochastSet->VaryingStochastSettings[i]->stochast));
                    }
                }
            }

            this->qualitativeCount = this->qualitativeIndices.size();
        }

        DesignPointBuilder::DesignPointBuilder(DesignPointMethod method, std::vector<std::shared_ptr<Statistics::Stochast>> stochasts)
        {
            initializeSamples(stochasts.size(), method);

            for (int i = 0; i < stochasts.size(); i++)
            {
                if (stochasts[i]->isQualitative())
                {
                    this->qualitativeIndices.push_back(i);
                    this->modeFinders.push_back(std::make_shared<ModeFinder>(stochasts[i]));
                }
            }

            this->qualitativeCount = this->qualitativeIndices.size();
        }

        void DesignPointBuilder::initializeSamples(int count, DesignPointMethod method)
        {
            this->count = count;
            this->method = method;

            defaultSample = std::make_shared<Sample>(count);
            meanSample = std::make_shared<Sample>(count);
            sinSample = std::make_shared<Sample>(count);
            cosSample = std::make_shared<Sample>(count);

            this->qualitativeIndices.clear();
            this->modeFinders.clear();

        }

        void DesignPointBuilder::initialize(double beta)
        {
            double value = Numeric::NumericSupport::GetSign(beta) * sqrt(std::abs(beta) / count);

            for (int i = 0; i < count; i++)
            {
                defaultSample->Values[i] = value;
            }
        }

        void DesignPointBuilder::addSample(std::shared_ptr<Sample> sample)
        {
            sampleAdded = true;

            switch (method)
            {
            case DesignPointMethod::NearestToMean:
            {
                double rbeta = sample->getBeta();
                if (rbeta < rmin)
                {
                    rmin = rbeta;

                    meanSample = sample->clone();
                }
                break;
            }
            case DesignPointMethod::CenterOfGravity:
            {
                const double weight = std::isnan(sample->Weight) ? 1 : sample->Weight;

                for (int j = 0; j < this->qualitativeCount; j++)
                {
                    int qIndex = qualitativeIndices[j];
                    modeFinders[j]->add(sample->Values[qIndex], weight);
                }

                for (int i = 0; i < sample->getSize(); i++)
                {
                    meanSample->Values[i] += weight * sample->Values[i];
                }

                sumWeights += weight;
                break;
            }
            case DesignPointMethod::CenterOfAngles:
            {
                const double weight = std::isnan(sample->Weight) ? 1 : sample->Weight;

                for (int j = 0; j < this->qualitativeCount; j++)
                {
                    int qIndex = qualitativeIndices[j];
                    modeFinders[j]->add(sample->Values[qIndex], weight);
                }

                auto sphericalValues = Numeric::NumericSupport::GetSphericalCoordinates(sample->Values);
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

        std::shared_ptr<Sample> DesignPointBuilder::getSample()
        {
            if (!sampleAdded)
            {
                return defaultSample->clone();
            }
            else
            {
                switch (method)
                {
                case DesignPointMethod::NearestToMean:
                {
                    return meanSample->clone();
                }
                case DesignPointMethod::CenterOfGravity:
                {
                    std::shared_ptr<Sample> gravityPoint = std::make_shared<Sample>(count);

                    for (int i = 0; i < count; i++)
                    {
                        gravityPoint->Values[i] = meanSample->Values[i] / sumWeights;
                    }

                    for (int j = 0; j < this->qualitativeCount; j++)
                    {
                        int qIndex = qualitativeIndices[j];
                        gravityPoint->Values[qIndex] = modeFinders[j]->getMode();
                    }

                    return gravityPoint;
                }
                case DesignPointMethod::CenterOfAngles:
                {
                    auto angleValues = std::vector<double>(count);
                    angleValues[0] = meanSample->Values[0] / sumWeights;
                    for (int i = 1; i < count; i++)
                    {
                        angleValues[i] = std::atan2(sinSample->Values[i] / sumWeights, cosSample->Values[i] / sumWeights);
                    }

                    auto coordinates = Numeric::NumericSupport::GetCartesianCoordinates(angleValues);
                    std::shared_ptr<Sample> anglePoint = std::make_shared<Sample>(coordinates);

                    for (int j = 0; j < this->qualitativeCount; j++)
                    {
                        int qIndex = qualitativeIndices[j];
                        anglePoint->Values[qIndex] = modeFinders[j]->getMode();
                    }

                    return anglePoint;
                }
                default:
                    throw std::runtime_error("Not supported");
                }
            }
        }

        std::string DesignPointBuilder::getDesignPointMethodString(DesignPointMethod method)
        {
            switch (method)
            {
            case DesignPointMethod::NearestToMean: return "nearest_to_mean";
            case DesignPointMethod::CenterOfGravity: return "center_of_gravity";
            case DesignPointMethod::CenterOfAngles: return "center_of_angles";
            default: throw probLibException("Design point method");
            }
        }

        DesignPointMethod DesignPointBuilder::getDesignPointMethod(std::string method)
        {
            if (method == "nearest_to_mean") return DesignPointMethod::NearestToMean;
            else if (method == "center_of_gravity") return DesignPointMethod::CenterOfGravity;
            else if (method == "center_of_angles") return DesignPointMethod::CenterOfAngles;
            else throw probLibException("Design point method");
        }
    }
}



