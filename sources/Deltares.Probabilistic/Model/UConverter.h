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

#include <vector>

#include "Sample.h"
#include "StochastPoint.h"
#include "ComputationalStochast.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Reliability/StochastSettingsSet.h"
#include "../Sensitivity/CorrelationMatrixBuilder.h"

namespace Deltares
{
    namespace Models
    {
        class UConverter
        {
        private:
            bool hasQualitiveStochasts = false;
            bool hasVariableStochasts = false;

            std::vector<std::shared_ptr<ComputationalStochast>> stochasts;
            std::vector<std::shared_ptr<ComputationalStochast>> varyingStochasts;
            std::vector<int> varyingStochastIndex;
            std::vector<int> pureVaryingStochastIndex;

            std::vector<int> variableStochastIndex; // reference of the stochast index to the variable source index
            std::vector<int> variableStochastList; // list of all stochast indices in order how they should be assigned

            std::shared_ptr<Statistics::CorrelationMatrix> correlationMatrix;
            std::shared_ptr<Statistics::CorrelationMatrix> varyingCorrelationMatrix;

            std::vector<double> getExpandedValues(const std::vector<double>& values);
            std::vector<double> getExpandedValues(const std::vector<double>& values, double defaultValue);

            bool isFullyCorrelated(const int index, std::vector<int> varyingIndices);
            void updateDependentParameter(std::vector<double>& uValues, const int i);
            std::shared_ptr<Sample> getQualitativeExcludedSample(std::shared_ptr<Sample> sample);
            std::vector<int> getVariableStochastIndex();

            bool sampleValuesChanged = false;

        public:

            UConverter(std::vector<std::shared_ptr<Deltares::Statistics::Stochast>> stochasts, std::shared_ptr<Statistics::CorrelationMatrix> stochastCorrelationMatrix);
            void initializeForRun();
            std::vector<double> getUValues(std::shared_ptr<Sample> sample);
            std::vector<double> getExpandedUValues(std::shared_ptr<Sample> sample);
            std::vector<double> getXValues(std::shared_ptr<Sample> sample);
            int getStochastCount();
            int getVaryingStochastCount();
            bool isVaryingStochast(int index);
            std::vector<double> getVaryingValues(std::vector<double> values);
            std::shared_ptr<StochastPoint> GetStochastPoint(std::shared_ptr<Sample> sample, double beta);
            std::shared_ptr<StochastPoint> GetStochastPoint(double beta, std::vector<double> alphas);
            void updateStochastSettings(std::shared_ptr<Deltares::Reliability::StochastSettingsSet> settings);
            void updateVariableSample(std::vector<double>& xValues, std::vector<double>& originalValues);
            void registerSample(std::shared_ptr<Sensitivity::CorrelationMatrixBuilder> correlationMatrixBuilder, std::shared_ptr<Sample> sample);
            bool haveSampleValuesChanged() const { return sampleValuesChanged; }
        };
    }
}

