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
#include "ExcludingCombineProject.h"
#include "DesignPointCombiner.h"

namespace Deltares::Reliability
{
    void ExcludingCombineProject::run()
    {
        std::shared_ptr<DesignPointCombiner> combiner = std::make_shared<DesignPointCombiner>();
        combiner->excludingCombinerType = this->settings->combinerMethod;

        std::unique_ptr<DesignPoint> designPointPtr = combiner->combineDesignPointsExcluding(this->scenarios, this->designPoints);
        this->designPoint = std::move(designPointPtr);
    }

    std::vector<std::shared_ptr<Models::Message>> ExcludingCombineProject::validate() const
    {
        const double margin = 1E-10;

        std::vector<std::shared_ptr<Models::Message>> messages;

        if (this->designPoints.size() != this->scenarios.size())
        {
            messages.push_back(std::make_shared<Message>(MessageType::Error, "Number of scenarios should be equal to number of design points."));
        }

        double sumProbabilities = 0;
        for (std::shared_ptr<Statistics::Scenario> scenario : this->scenarios)
        {
            sumProbabilities += scenario->probability;
            for (std::shared_ptr<Message> scenarioMessage : scenario->validate())
            {
                messages.push_back(scenarioMessage);
            }
        }

        if (std::abs(1 - sumProbabilities) > margin)
        {
            messages.push_back(std::make_shared<Message>(MessageType::Error, "Scenario probabilities should add upp to 1."));
        }

        return messages;
    }

    bool ExcludingCombineProject::is_valid() const
    {
        return this->validate().empty();
    }
}

