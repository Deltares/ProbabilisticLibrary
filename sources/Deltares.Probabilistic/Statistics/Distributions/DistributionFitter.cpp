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
#include "DistributionFitter.h"
#include "Distribution.h"
#include "../Stochast.h"
#include "../../Optimization/GridSearch.h"
#include "../../Optimization/SearchParameterSettings.h"
#include "../../Optimization/SearchParameterSettingsSet.h"
#include "../../Model/ModelSample.h"
#include "../../Model/ZModel.h"
#include "../../Math/NumericSupport.h"

#include <memory>
#include <vector>

namespace Deltares
{
    namespace Statistics
    {
        std::vector<double> DistributionFitter::fitByLogLikelihood(const std::vector<double>& values, Distribution* distribution, StochastProperties& stochast,
            const std::vector<double>& minimum, const std::vector<double>& maximum, const std::vector<DistributionPropertyType>& properties)
        {
            distributionObj = distribution;

            constexpr int numberValues = 13;
            constexpr int numberRefinements = 10;

            auto gridSearch = Optimization::GridSearch();

            auto searchArea = std::make_shared<Optimization::SearchParameterSettingsSet>();
            for (size_t i = 0; i < properties.size(); i++)
            {
                auto settings = std::make_shared<Optimization::SearchParameterSettings>();
                settings->MinValue = minimum[i];
                settings->MaxValue = maximum[i];
                settings->NumberOfValues = numberValues;
                settings->NumberOfRefinements = numberRefinements;
                settings->Move = true;
                searchArea->Dimensions.push_back(settings);
            }

            const auto model = std::make_shared<Models::ZModel>([this, values, &stochast, properties]
                (const std::shared_ptr<Models::ModelSample>& sample)
                { return getLogLikelihood(*sample, values, stochast, properties); });

            const std::shared_ptr<Models::ModelSample> sample = gridSearch.getOptimizedSample(searchArea, model);

            if (sample != nullptr)
            {
                return sample->Values;
            }
            else
            {
                return Numeric::NumericSupport::select(minimum, [](double x) {return std::nan(""); });
            }
        }

        void DistributionFitter::getLogLikelihood(Models::ModelSample& sample, const std::vector<double>& values,
            StochastProperties& stochast, const std::vector<DistributionPropertyType>& properties) const
        {
            for (size_t i = 0; i < properties.size(); i++)
            {
                stochast.applyValue(properties[i], sample.Values[i]);
            }

            if (distributionObj->isValid(stochast))
            {
                sample.Z = - getSumLogLikelihood(values, stochast);
            }
            else
            {
                sample.Z = nan("");
            }
        }

        double DistributionFitter::getSumLogLikelihood(const std::vector<double>& values, StochastProperties& stochast) const
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
                    double log = distributionObj->getLogLikelihood(stochast, x);
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


