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
#include "OptimizationSettings.h"
#include "../Utils/ProbabilisticLibraryException.h"

#include <memory>

namespace Deltares::Optimization
{
    using enum OptimizationMethodType;

    std::shared_ptr<OptimizationMethod> OptimizationSettings::GetOptimizationMethod() const
    {
        switch (this->OptimizationMethod)
        {
        case GridSearch: return this->GetGridSearchMethod();
        case Cobyla: return this->GetCobylaMethod();
        case APSO: return this->GetApsoMethod();

        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    std::shared_ptr<GridSearchOptimization> OptimizationSettings::GetGridSearchMethod() const
    {
        std::shared_ptr<GridSearchOptimization> gridSearch = std::make_shared<GridSearchOptimization>();

        gridSearch->Settings.MaxGridMoves = this->MaxGridMoves;
        gridSearch->Settings.RunSettings = this->RunSettings;
        gridSearch->Settings.SearchArea = this->SearchArea;

        return gridSearch;
    }

    std::shared_ptr<CobylaOptimization> OptimizationSettings::GetCobylaMethod() const
    {
        std::shared_ptr<CobylaOptimization> cobyla = std::make_shared<CobylaOptimization>();

        cobyla->Settings.EpsilonBeta = this->EpsilonBeta;
        cobyla->Settings.MaxIterations = this->Iterations;
        cobyla->Settings.SearchArea = this->SearchArea;

        return cobyla;
    }

    std::shared_ptr<AdaptiveParticleSwarmOptimization> OptimizationSettings::GetApsoMethod() const
    {
        auto apso = std::make_shared<AdaptiveParticleSwarmOptimization>();

        apso->Options.SearchArea = *SearchArea;

        return apso;
    }

    /**
     * \brief Reports whether the settings have valid values
     * \param report Report in which the validity is reported
     */
    void OptimizationSettings::validate(Logging::ValidationReport& report) const
    {
        switch (OptimizationMethod)
        {
        case GridSearch: GetGridSearchMethod()->Settings.validate(report); break;
        case Cobyla: GetCobylaMethod()->Settings.validate(report); break;
        case APSO: GetApsoMethod()->Options.validate(report); break;
        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    std::string OptimizationSettings::getOptimizationMethodTypeString(OptimizationMethodType method)
    {
        switch (method)
        {
        case GridSearch: return "grid";
        case Cobyla: return "cobyla";
        case APSO: return "APSO";
        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    OptimizationMethodType OptimizationSettings::getOptimizationMethodType(const std::string& method)
    {
        if (method == "grid") return GridSearch;
        else if (method == "cobyla") return Cobyla;
        else if (method == "APSO") return APSO;
        else throw Reliability::ProbabilisticLibraryException("Optimization method");
    }
}

