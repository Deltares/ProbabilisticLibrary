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

#pragma once

#include "SearchParameterSettingsSet.h"
#include "ThreeFractions.h"
#include "../Model/Validatable.h"

namespace Deltares::Optimization
{
    enum class InitializePopulationMethod { Uniform, Parabolic };
    enum class RandomFrequency { Never, Generation, Child };
    enum class RandomEffect { Unity, Free };

    /**
     * \brief Settings for the C++ port of GeneticAlgorithm.cs.
     * \remark Each group of crossover or mutation fractions must sum to one;
     * unlike the C# properties, assigning a fraction does not change the others.
     */
    class GeneticAlgorithmSettings : public Models::Validatable
    {
    public:
        SearchParameterSettingsSet SearchArea;
        int EliteCount = 2;
        int PopulationCount = 60;
        int GenerationCount = 50;
        double MutationRate = 0.3;
        ThreeFractions CrossOverFractions = ThreeFractions(0.0, 0.7, 0.3);
        ThreeFractions MutationFractions = ThreeFractions(0.0, 0.9, 0.1);
        double MutationCreepReduction = 0.05;
        int Seed = 1;
        InitializePopulationMethod initializePopulationMethod = InitializePopulationMethod::Uniform;
        RandomFrequency CrossOverRandomFrequency = RandomFrequency::Never;
        RandomEffect CrossOverRandomEffect = RandomEffect::Free;
        RandomFrequency MutationRandomFrequency = RandomFrequency::Never;
        RandomEffect MutationRandomEffect = RandomEffect::Free;

        void validate(Logging::ValidationReport& report) const override;
    };
}
