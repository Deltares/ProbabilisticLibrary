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
#pragma once
#include <vector>
#include "basicSettings.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"
#include "../Deltares.Probabilistic/Model/RandomSettings.h"
#include "../Deltares.Probabilistic/Reliability/FORMSettings.h"
#include "../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Deltares.Probabilistic/Reliability/ImportanceSamplingSettings.h"

class createReliabilityMethod
{
public:
    static std::shared_ptr<Deltares::Reliability::ReliabilityMethod> selectMethod(const Deltares::ProbLibCore::basicSettings& bs,
        const size_t nStoch, std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts);

private:
    static std::shared_ptr<Deltares::Models::RandomSettings> getRnd(const Deltares::ProbLibCore::basicSettings& bs);
    static std::vector<double> copyStartVector(const double startVector[], const size_t nStoch);
    static void fillFormSettings(std::shared_ptr<Deltares::Reliability::FORMSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
    static void fillDsSettings(std::shared_ptr<Deltares::Reliability::DirectionalSamplingSettings>& Settings, const Deltares::ProbLibCore::basicSettings& bs);
    static Deltares::Reliability::DesignPointMethod convertDp(const Deltares::ProbLibCore::DPoptions dp);
    static void fillImportanceSamplingSettings(std::shared_ptr<Deltares::Reliability::ImportanceSamplingSettings>& settings, const Deltares::ProbLibCore::basicSettings& bs,
        std::vector<std::shared_ptr<Deltares::Statistics::Stochast>>& stochasts);
    static void fillStartVector(std::shared_ptr<Deltares::Reliability::StartPointCalculatorSettings>& startPoint,
        const Deltares::ProbLibCore::basicSettings& bs, const size_t nStoch);
};

