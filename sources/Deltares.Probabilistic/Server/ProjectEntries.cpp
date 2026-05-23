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
#include "ProjectEntries.h"
#include "../Utils/probLibException.h"

namespace Deltares::Server
{
    using enum ObjectType;

    const std::map<std::string, ObjectType,std::less<>> ProjectEntries::all_entries = {
        {"standard_normal", StandardNormal},
        {"probability_value", ProbabilityValue},
        {"message", Message},
        {"validation_report", ValidationReport},
        {"project", Project},
        {"model_parameter", ModelParameter},
        {"limit_state_function", LimitStateFunction},
        {"combined_limit_state_function", CombinedLimitStateFunction},
        {"probability_limit_state_function", ProbabilityLimitStateFunction},
        {"stochast", Stochast},
        {"discrete_value", DiscreteValue},
        {"histogram_value", HistogramValue},
        {"fragility_value", FragilityValue},
        {"contributing_stochast", ContributingStochast},
        {"conditional_value", ConditionalValue},
        {"correlation_matrix", CorrelationMatrix},
        {"copula_correlation", CopulaCorrelation},
        {"scenario", Scenario},
        {"settings", Settings},
        {"stochast_settings", StochastSettings},
        {"stochast_point", StochastPoint},
        {"design_point", DesignPoint},
        {"alpha", Alpha},
        {"fragility_curve", FragilityCurve},
        {"fragility_curve_project", FragilityCurveProject},
        {"fragility_curve_settings", FragilityCurveSettings},
        {"evaluation", Evaluation},
        {"reliability_result", ReliabilityResult},
        {"combine_project", CombineProject},
        {"combine_settings", CombineSettings},
        {"excluding_combine_project", ExcludingCombineProject},
        {"excluding_combine_settings", ExcludingCombineSettings},
        {"self_correlation_matrix", SelfCorrelationMatrix},
        {"run_project", RunProject},
        {"run_project_settings", RunProjectSettings},
        {"sensitivity_project", SensitivityProject},
        {"sensitivity_settings", SensitivitySettings},
        {"sensitivity_result", SensitivityResult},
        {"sensitivity_value", SensitivityValue},
        {"uncertainty_project", UncertaintyProject},
        {"uncertainty_settings", UncertaintySettings},
        {"uncertainty_result", UncertaintyResult},
        {"length_effect_project", LengthEffectProject},
        {"convergence_report", ConvergenceReport}
    };

    ObjectType ProjectEntries::GetType(const std::string& object_type)
    {
        const auto it = all_entries.find(object_type);
        if (it != all_entries.end())
        {
            return it->second;
        }
        throw Reliability::probLibException("type not supported: " + object_type);
    }

    bool ProjectEntries::CanHandle(const std::string& object_type)
    {
        return all_entries.contains(object_type);
    }

}


