#pragma once

#include <string>
#include <vector>

#include "../Model/Evaluation.h"
#include "../Model/Message.h"
#include "../Model/StochastPoint.h"
#include "ReliabilityResult.h"
#include "ConvergenceReport.h"

namespace Deltares
{
	namespace Reliability
	{
		using namespace Deltares::Models;

		class DesignPoint : public StochastPoint
		{
		public:
			std::string Identifier = "";
			int ScenarioIndex = -1;

			std::vector<std::shared_ptr<DesignPoint>> ContributingDesignPoints;
			std::vector<std::shared_ptr<ReliabilityResult>> ReliabililityResults;
			std::vector<std::shared_ptr<Evaluation>> Evaluations;
			std::vector<std::shared_ptr<Deltares::Models::Message>> Messages;

			std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();
		};
	}
}

