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

#include <memory>

namespace Deltares::Sensitivity
{
    class Sobol;
}

namespace Deltares::Optimization
{
    std::shared_ptr<OptimizationMethod> OptimizationSettings::GetOptimizationMethod()
    {
        switch (this->OptimizationMethod)
        {
        case OptimizationMethodType::GridSearch: return this->GetGridSearchMethod();
        case OptimizationMethodType::Cobyla: return this->GetCobylaMethod();

        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    std::shared_ptr<GridSearch> OptimizationSettings::GetGridSearchMethod() const
    {
        std::shared_ptr<GridSearch> gridSearch = std::make_shared<GridSearch>();

        gridSearch->Settings->MaxGridMoves = this->MaxGridMoves;
        gridSearch->Settings->RunSettings = this->RunSettings;

        return gridSearch;
    }

    std::shared_ptr<CobylaOptimization> OptimizationSettings::GetCobylaMethod() const
    {
        std::shared_ptr<CobylaOptimization> cobyla = std::make_shared<CobylaOptimization>();

        cobyla->Settings->EpsilonBeta = this->EpsilonBeta;
        cobyla->Settings->MaxIterations = this->Iterations;
        cobyla->Settings->SearchArea = this->SearchArea;

        return cobyla;
    }

    /**
     * \brief Reports whether the settings have valid values
     * \param report Report in which the validity is reported
     */
    void OptimizationSettings::validate(Logging::ValidationReport& report) const
    {
        switch (this->OptimizationMethod)
        {
        case OptimizationMethodType::GridSearch: GetGridSearchMethod()->Settings->validate(report); break;
        case OptimizationMethodType::Cobyla: GetCobylaMethod()->Settings->validate(report); break;
        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    std::string OptimizationSettings::getOptimizationMethodTypeString(OptimizationMethodType method)
    {
        switch (method)
        {
        case OptimizationMethodType::GridSearch: return "grid";
        case OptimizationMethodType::Cobyla: return "cobyla";
        default: throw Reliability::ProbabilisticLibraryException("Optimization method");
        }
    }

    OptimizationMethodType OptimizationSettings::getOptimizationMethodType(std::string method)
    {
        if (method == "grid") return OptimizationMethodType::GridSearch;
        else if (method == "cobyla") return OptimizationMethodType::Cobyla;
        else throw Reliability::ProbabilisticLibraryException("Optimization method");
    }
}


