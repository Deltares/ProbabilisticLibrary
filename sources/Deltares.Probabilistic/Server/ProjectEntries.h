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

#include <map>
#include <string>

namespace Deltares::Server
{
    enum class ObjectType {
        StandardNormal,
        Message,
        ValidationReport,
        ProbabilityValue,
        Project,
        ModelParameter,
        LimitStateFunction,
        CombinedLimitStateFunction,
        ProbabilityLimitStateFunction,
        Stochast,
        DiscreteValue,
        HistogramValue,
        FragilityValue,
        ContributingStochast,
        ConditionalValue,
        CorrelationMatrix,
        Scenario,
        Settings,
        StochastSettings,
        StochastPoint,
        DesignPoint,
        Alpha,
        FragilityCurve,
        FragilityCurveProject,
        FragilityCurveSettings,
        Evaluation,
        CombineProject,
        CombineSettings,
        ExcludingCombineProject,
        ExcludingCombineSettings,
        SelfCorrelationMatrix,
        UncertaintyProject,
        UncertaintySettings,
        UncertaintyResult,
        SensitivityProject,
        SensitivitySettings,
        SensitivityResult,
        SensitivityValue,
        LengthEffectProject,
        RunProject,
        RunProjectSettings,
        ReliabilityResult,
        CopulaCorrelation,
        ConvergenceReport,
    };

    /// <summary>
    /// Helper class for ProjectHandler
    /// </summary>
    class ProjectEntries
    {
    public:
        /// <summary>
        /// Checks whether the object_type is available
        /// </summary>
        /// <param name="object_type"> string in lower case with object name </param>
        /// <returns> true if available </returns>
        static bool CanHandle(const std::string& object_type);

        /// <summary>
        /// convert string to enum ObjectType
        /// </summary>
        /// <param name="object_type"> string in lower case with object name </param>
        /// <returns> enum corresponding with name </returns>
        /// <exception cref="probLibException"> object name not available </exception>
        static ObjectType GetType(const std::string& object_type);

        /// <summary>
        /// checks if object type is one of the model projects
        /// </summary>
        /// <param name="object_type"> enum object type </param>
        /// <returns> true if it is a model project </returns>
        static bool IsModelProjectType(ObjectType object_type);

        /// <summary>
        /// checks if object type is one of the model settings type
        /// </summary>
        /// <param name="object_type"> enum object type </param>
        /// <returns> true if it is a model settings type </returns>
        static bool IsModelSettingsType(ObjectType object_type);

        /// <summary>
        /// checks if object type is one of the stochast types
        /// </summary>
        /// <param name="object_type"> enum object type </param>
        /// <returns> true if it is a stochast type </returns>
        static bool IsStochast(ObjectType object_type);
    private:
        static const std::map<std::string, ObjectType, std::less<>> all_entries;
    };
}
