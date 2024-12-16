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
#include "DistributionFitter.h"
#include "Distribution.h"
#include "../Stochast.h"
#include "../../Optimization/GridSearch.h"
#include "../../Optimization/SearchParameterSettings.h"
#include "../../Optimization/SearchParameterSettingsSet.h"
#include "../../Model/ModelSample.h"
#include "../../Model/ZModel.h"

#include <memory>
#include <vector>

namespace Deltares
{
    namespace Statistics
    {
        std::vector<double> DistributionFitter::fitByLogLikelihood(std::vector<double>& values, Distribution* distribution, std::shared_ptr<StochastProperties> stochast, std::vector<double>& minimum, std::vector<double>& maximum, std::vector<double>& initial, std::vector<DistributionPropertyType>& properties)
        {
            this->stochast = stochast;
            this->values = values;
            this->distribution = distribution;
            this->properties = properties;

            constexpr int numberValues = 13;
            constexpr int numberRefinements = 10;

            std::shared_ptr<Optimization::GridSearch> gridSearch = std::make_shared<Optimization::GridSearch>();

            std::shared_ptr<Optimization::SearchParameterSettingsSet> searchArea = std::make_shared<Optimization::SearchParameterSettingsSet>();
            for (int i = 0; i < properties.size(); i++)
            {
                std::shared_ptr<Optimization::SearchParameterSettings> settings = std::make_shared<Optimization::SearchParameterSettings>();
                settings->MinValue = minimum[i];
                settings->MaxValue = maximum[i];
                settings->NumberOfValues = numberValues;
                settings->NumberOfRefinements = numberRefinements;
                settings->Move = true;
                searchArea->Dimensions.push_back(settings);
            }

            DistributionFitter* fitter = this;

            const std::shared_ptr<Models::ZModel> model(new Models::ZModel([fitter](std::shared_ptr<Models::ModelSample> sample) { return fitter->getLogLikelihood(sample); }));

            const std::shared_ptr<Models::ModelSample> sample = gridSearch->getOptimizedSample(searchArea, model);

            return sample->Values;
        }

        void DistributionFitter::getLogLikelihood(std::shared_ptr<Models::ModelSample> sample)
        {
            for (int i = 0; i < properties.size(); i++)
            {
                this->stochast->applyValue(properties[i], sample->Values[i]);
            }

            if (distribution->isValid(stochast))
            {
                sample->Z = - this->getSumLogLikelihood();
            }
            else
            {
                sample->Z = nan("");
            }
        }

        double DistributionFitter::getSumLogLikelihood()
        {
            double prevLog = NAN;
            double prevX = NAN;

            double sum = 0;

            for (double x : values)
            {
                if (x == prevX)
                {
                    sum += prevLog;
                }
                else
                {
                    double log = this->distribution->getLogLikelihood(this->stochast, x);
                    if (std::isnan(log))
                    {
                        return log;
                    }
                    else
                    {
                        prevX = x;
                        prevLog = log;

                        sum += log;
                    }
                }
            }

            return sum;
        }

    }
}


