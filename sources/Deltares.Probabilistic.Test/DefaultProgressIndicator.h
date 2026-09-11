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

#include <string>

#include "../Deltares.Probabilistic/Model/ProgressIndicator.h"

namespace Deltares::Probabilistic::Test
{
    class DefaultProgressIndicator
    {
    public:
        int invocations = 0;
        double last_progress = std::nan("");
        int last_step = -1;
        int last_max_steps = -1;
        double last_reliability = std::nan("");
        double last_convergence = std::nan("");
        std::string last_message = "";

        void doProgress(double progress)
        {
            invocations++;
            last_progress = progress;
        }

        void doDetailedProgress(int step, int max_steps, double reliability, double convergence)
        {
            last_step = step;
            last_max_steps = max_steps;
            last_reliability = reliability;
            last_convergence = convergence;
        }

        void doTextualProgress(Models::ProgressType progress_type, const char* message)
        {
            last_message = std::string(message);
        }
    };
}
    
