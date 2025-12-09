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

#include "../Model/RunSettings.h"
#include "../Model/Validatable.h"
#include "../Model/ModelSample.h"
#include "../Utils/probLibException.h"

namespace Deltares::Proxies
{
    enum class ProxyInitializationType
    {
        Single,
        SingleAndFullFactorial,
        Double,
        ExperimentalDesign,
        FullFactorial,
        Random
    };

    enum class ProxyMethodType
    {
        FirstOrder,
        SecondOrder,
        LinearGrid,
        GPR
    };

    enum class QualitativeValue
    {
        Low,
        Medium,
        High
    };

    /**
     * \brief Settings for FORM algorithm
     */
    class ProxySettings : public Models::Validatable
    {
    public:
        /**
         * \brief Temporary: indicates whether a proxy model is used
         */
        bool IsProxyModel = false;

        /**
         * \brief Temporary: indicates whether thr proxy is updatable
         */
        bool IsUpdatableProxyModel = false;

        /**
         * \brief Temporary: indicates update info
         */

        bool ShouldUpdateFinalSteps = false;

        /**
         * \brief Temporary: Threshold value
         */
        double ThresholdOffset = 0;

        /**
         * \brief Relaxation factor, which is applied when generating the guessed design point for a new iteration
         */
        double  VarianceFactor = 1.0;

        /**
         * \brief Indicates whether training samples should have a small deviation
         */
        bool Disturbed = true;

        /**
         * \brief Maximum number of proxy loops when no convergence is reached
         */
        int NumberOfRuns = 10;

        /**
         * \brief Random generator seed
         */
        int Seed = 0;

        /**
         * \brief Indicates in a single variation training set whether a full factorial combination should be added
         */
        bool AddFullFactorial = false;

        /**
         * \brief Indicates the direction where initial values should be retrieved
         */
        QualitativeValue Direction = QualitativeValue::Low;

        /**
         * \brief Gets the u-value corresponding to the low qualitative definition
         */
        double LowDefinition = 0.1;

        /**
         * \brief Gets the u-value corresponding to the high qualitative definition
         */
        double HighDefinition = 0.9;

        /**
         * \brief Gets the u-value corresponding to a qualitative value
         */
        double GetQualitativeValue(QualitativeValue qualitativeValue) const
        {
            switch (qualitativeValue)
            {
            case QualitativeValue::Low: return LowDefinition;
            case QualitativeValue::High: return HighDefinition;
            case QualitativeValue::Medium: return 0.5;
            default: throw Reliability::probLibException("Qualitative value");
            }
        }

        /**
         * \brief Indicates how initial training samples are generated
         */
        ProxyInitializationType InitializationType = ProxyInitializationType::Single;

        /**
         * \brief If true, makes a guess for the design point if the final FORM loop does not converge
         */
        ProxyMethodType MethodType = ProxyMethodType::FirstOrder;

        /**
         * \brief Settings for performing model runs
         */
        std::vector<std::shared_ptr<Models::ModelSample>> TrainingSamples;

        void validate(Logging::ValidationReport& report) const override
        {
            Logging::ValidationSupport::checkMinimumInt(report, 1, NumberOfRuns, "number of runs");
            Logging::ValidationSupport::checkMinimum(report, 0.01, VarianceFactor, "variance factor");
            if (IsProxyModel)
            {
                Logging::ValidationSupport::checkMinimum(report, 0, ThresholdOffset, "threshold offset");
            }
        }
    };
}

