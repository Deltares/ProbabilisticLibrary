#pragma once

#include <string>
#include <vector>

#include "../Model/Evaluation.h"
#include "../Model/Message.h"
#include "../Model/StochastPoint.h"
#include "../Statistics/StandardNormal.h"
#include "ReliabilityResult.h"
#include "ConvergenceReport.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Models;

        /**
         * \brief Design point, result of a reliability calculation
         */
        class DesignPoint : public StochastPoint
        {
        public:
            std::string Identifier = "";

            std::vector<std::shared_ptr<DesignPoint>> ContributingDesignPoints;
            std::vector<std::shared_ptr<ReliabilityResult>> ReliabililityResults;
            std::vector<std::shared_ptr<Evaluation>> Evaluations;
            std::vector<std::shared_ptr<Deltares::Models::Message>> Messages;

            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            double getFailureProbability() { return Statistics::StandardNormal::getQFromU(this->Beta); }
            double getNonFailureProbability() { return Statistics::StandardNormal::getPFromU(this->Beta); }
        };
    }
}

