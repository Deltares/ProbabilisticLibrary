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

#include <memory>
#include "../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../Deltares.Probabilistic/Reliability/ReliabilityProject.h"
#include "../Deltares.Probabilistic/Sensitivity/SensitivityProject.h"

namespace Deltares
{
    namespace Probabilistic
    {
        namespace Test
        {
            class projectBuilder
            {
            public:
                std::shared_ptr<Deltares::Models::ModelRunner> BuildProject();
                std::shared_ptr<Deltares::Models::ModelRunner> BuildProjectWithDeterminist();

                static std::shared_ptr<Sensitivity::SensitivityProject> getSensitivityProject(std::shared_ptr<Deltares::Reliability::ReliabilityProject> project);
                static std::shared_ptr<Deltares::Reliability::ReliabilityProject> getAddOneProject();
                static std::shared_ptr<Deltares::Reliability::ReliabilityProject> getLinearProject();

                static const bool logZtoScreen = false;
            private:
                void zfunc(std::shared_ptr<Deltares::Models::ModelSample> sample) const;
                void zfuncWithDeterminist(std::shared_ptr<Deltares::Models::ModelSample> sample) const;

                static void sum(std::shared_ptr<Models::ModelSample> sample);
                static void linear(std::shared_ptr<Models::ModelSample> sample);

                static std::shared_ptr<Statistics::Stochast>  getDeterministicStochast(double mean = 0);
                static std::shared_ptr<Statistics::Stochast>  getNormalStochast(double mean = 0, double stddev = 1);
                static std::shared_ptr<Statistics::Stochast>  getLogNormalStochast(double mean = 0, double stddev = 1, double shift = 0);
                static std::shared_ptr<Statistics::Stochast>  getUniformStochast(double min = 0, double max = 1);
                static std::shared_ptr<Statistics::Stochast>  getGumbelStochast(double mean = 0, double stddev = 1);
            };
        }
    }
}
