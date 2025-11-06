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
#pragma once

#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "DesignPointBuilder.h"
#include "StartPointCalculatorSettings.h"
#include "StochastSettingsSet.h"
#include "../Model/Validatable.h"

namespace Deltares
{
    namespace Reliability
    {
        class ImportanceSamplingSettings : public Models::Validatable
        {
        public:
            /**
             * \brief The minimum number of samples to be examined
             */
            int MinimumSamples = 1000;

            /**
             * \brief The maximum number of samples to be examined
             */
            int MaximumSamples = 10000;

            /**
             * \brief Maximum number of samples when all samples have the same qualitative result (Z < 0 or Z > 0)
             */
            int MaximumSamplesNoResult = 10000;

            /**
             * \brief The importance sampling algorithm stops when the calculated variation coefficient is less than this value
             */
            double VariationCoefficient = 0.05;

            double VarianceFactor = 1;

            /**
             * \brief Method type how the design point (alpha values) is calculated
             */
            DesignPointMethod designPointMethod = DesignPointMethod::CenterOfGravity;

            /**
             * \brief Settings how to derive the first center
             */
            std::shared_ptr<StartPointCalculatorSettings> startPointSettings = std::make_shared<StartPointCalculatorSettings>();

            /**
             * \brief Settings for performing model runs
             */
            std::shared_ptr<Models::RunSettings> runSettings = std::make_shared<Models::RunSettings>();

            /**
             * \brief Settings per stochastic variable, contains (among others) the center value and multiplication factor used to shift samples in the importance sampling algorithm
             */
            std::shared_ptr<StochastSettingsSet> StochastSet = std::make_shared<StochastSettingsSet>();

            std::shared_ptr<Deltares::Models::RandomSettings> randomSettings = std::make_shared<Deltares::Models::RandomSettings>();

            bool Clustering = false;
            std::vector<std::shared_ptr<Sample>> Clusters;

            void validateStochastSettings(Logging::ValidationReport& report) const
            {
                for (int i = 0; i < this->StochastSet->getVaryingStochastCount(); i++)
                {
                    std::shared_ptr<StochastSettings> stochastSettings = this->StochastSet->VaryingStochastSettings[i];

                    Logging::ValidationSupport::checkMinimum(report, 0.01, stochastSettings->VarianceFactor, "variance factor");
                    Logging::ValidationSupport::checkMinimum(report, -Statistics::StandardNormal::UMax, stochastSettings->StartValue, "start value");
                    Logging::ValidationSupport::checkMaximum(report, Statistics::StandardNormal::UMax, stochastSettings->StartValue, "start value");
                }
            }

            /**
             * \brief Reports whether the settings have valid values
             * \param report Report in which the validity is reported
             * \return Indication
             */
            void validate(Logging::ValidationReport& report) const override
            {
                Logging::ValidationSupport::checkMinimumInt(report, 1, MinimumSamples, "minimum samples");
                Logging::ValidationSupport::checkMinimumInt(report, MinimumSamples, MaximumSamples, "maximum samples");
                Logging::ValidationSupport::checkMinimum(report, 0.01, VarianceFactor, "variance factor");
                Logging::ValidationSupport::checkMinimum(report, 0, VariationCoefficient, "variation coefficient");

                validateStochastSettings(report);

                runSettings->validate(report);
            }

            /**
             * \brief Makes a copy of these settings (deep copy of stochast settings)
             * \return Copy
             */
            std::shared_ptr<ImportanceSamplingSettings> clone()
            {
                std::shared_ptr<ImportanceSamplingSettings> clone = std::make_shared<ImportanceSamplingSettings>();

                clone->MaximumSamples = this->MaximumSamples;
                clone->MaximumSamplesNoResult = this->MaximumSamplesNoResult;
                clone->MinimumSamples = this->MinimumSamples;
                clone->VarianceFactor = this->VarianceFactor;
                clone->VariationCoefficient = this->VariationCoefficient;
                clone->designPointMethod = this->designPointMethod;

                // move to adaptive importance sampling settings
                clone->Clustering = this->Clustering;

                clone->StochastSet->AreStartValuesCorrelated = this->StochastSet->AreStartValuesCorrelated;

                clone->runSettings = this->runSettings;
                clone->randomSettings = this->randomSettings;
                clone->startPointSettings = this->startPointSettings->clone();

                for (size_t i = 0; i < this->StochastSet->getStochastCount(); i++)
                {
                    clone->StochastSet->stochastSettings.push_back(this->StochastSet->stochastSettings[i]->clone());
                }

                return clone;
            }
        };
    }
}

