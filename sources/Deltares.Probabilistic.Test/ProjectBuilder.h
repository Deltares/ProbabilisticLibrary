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

#include <memory>
#include "../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../Deltares.Probabilistic/Model/RunProject.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityProject.h"
#include "../Deltares.Probabilistic/Uncertainty/UncertaintyProject.h"
#include "../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"
#include "../Deltares.Probabilistic/Reliability/FragilityCurve.h"

namespace Deltares::Probabilistic::Test
{
    class ProjectBuilder
    {
    public:
        std::shared_ptr<Models::ModelRunner> BuildProject() const;
        static std::shared_ptr<Models::ModelRunner> BuildLinearProject(size_t nStochasts);
        static std::shared_ptr<Models::ModelRunner> BuildLinearProbabilityProject(size_t nStochasts);
        static std::shared_ptr<Models::ModelRunner> BuildLinearProbabilityInverseProject(size_t nStochasts);
        static std::shared_ptr<Models::ModelRunner> BuildLinearReliabilityProject(size_t nStochasts);
        static std::shared_ptr<Models::ModelRunner> BuildLinearReliabilityInverseProject(size_t nStochasts);
        static std::shared_ptr<Models::ModelRunner> BuildLinearOutputOnlyProject();
        static std::shared_ptr<Models::ModelRunner> BuildLinearOutputProject();
        static std::shared_ptr<Models::ModelRunner> BuildLinearArrayProject();
        static std::shared_ptr<Models::ModelRunner> BuildLinearVaryingArrayProject();
        static std::shared_ptr<Models::ModelRunner> BuildQuadraticProject();
        static std::shared_ptr<Models::ModelRunner> getQualitativeProject();
        std::shared_ptr<Models::ModelRunner> BuildProjectWithDeterminist(double valueDeterminist) const;
        std::shared_ptr<Models::ModelRunner> BuildProjectWithDeterministAndCopula(double valueDeterminist) const;
        std::shared_ptr<Models::ModelRunner> BuildProjectWithPolynome() const;
        std::shared_ptr<Models::ModelRunner> BuildProjectWithPolynome2() const;
        static std::shared_ptr<Reliability::FragilityCurve> BuildFragilityCurve();
        std::shared_ptr<Models::ModelRunner> BuildProjectTwoBranches(bool useProxy) const;

        static std::shared_ptr<Uncertainty::UncertaintyProject> getUncertaintyProject(std::shared_ptr<Reliability::ReliabilityProject> project);
        static std::shared_ptr<Sensitivity::SensitivityProject> getSensitivityProject(std::shared_ptr<Reliability::ReliabilityProject> project);
        static std::shared_ptr<Models::RunProject> getRunProject(std::shared_ptr<Deltares::Reliability::ReliabilityProject> project);
        static std::shared_ptr<Reliability::ReliabilityProject> getAddOneProject();
        static std::shared_ptr<Reliability::ReliabilityProject> getLinearProject();
        static std::shared_ptr<Reliability::ReliabilityProject> getLinearOutputProject();
        static std::shared_ptr<Reliability::ReliabilityProject> getTriangularLinearProject();
        static std::shared_ptr<Reliability::ReliabilityProject> getArrayVariableProject();

        static const bool logZtoScreen = false;
        static std::shared_ptr<Statistics::Stochast>  getDeterministicStochast(double mean = 0);
        static std::shared_ptr<Statistics::Stochast>  getNormalStochast(double mean = 0, double stddev = 1);
        static std::shared_ptr<Statistics::Stochast>  getLogNormalStochast(double mean = 0, double stddev = 1, double shift = 0);
    private:
        static std::shared_ptr<Models::ModelRunner> CreateModelRunner(size_t nStochasts, std::shared_ptr<Models::ZModel> zModel);

        void zfunc(Models::ModelSample& sample) const;
        void zfuncWithDeterminist(Models::ModelSample& sample) const;
        void zfuncTwoBranches(Models::ModelSample& sample) const;
        void zfuncTwoBranchesProxy(Models::ModelSample& sample) const;
        void zfuncPolynome(Models::ModelSample& sample) const;
        static void zfuncDiscrete(Models::ModelSample& sample);

        static void sum(Models::ModelSample& sample);
        static void linear(Models::ModelSample& sample);
        static void linearAutoStart(Models::ModelSample& sample);
        static void linearProbability(Models::ModelSample& sample);
        static void linearReliability(Models::ModelSample& sample);
        static void linearOutputOnly(Models::ModelSample& sample);
        static void linearMultiple(Models::ModelSample& sample);
        static void quadratic(Models::ModelSample& sample);

        static std::shared_ptr<Statistics::Stochast>  getUniformStochast(double min = 0, double max = 1);
        static std::shared_ptr<Statistics::Stochast>  getUniformVariableStochast(const std::shared_ptr<Statistics::Stochast>& source, double min, double max);

        static std::shared_ptr<Statistics::Stochast>  getTriangularStochast(double min = 0, double top = 0, double max = 1);
        static std::shared_ptr<Statistics::Stochast>  getGumbelStochast(double mean = 0, double stddev = 1);
    };
}
