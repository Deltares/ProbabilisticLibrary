#pragma once
#include <string>

#include "Sample.h"
#include <vector>

#include "ReliabilityReport.h"
#include "ReliabilityResult.h"
#include "RunSettings.h"
#include "UConverter.h"
#include "ZModel.h"
#include "DesignPoint.h"
#include "Message.h"
#include "ProgressIndicator.h"
#include "../Reliability/StochastSettingsSet.h"

namespace Deltares
{
	namespace Models
	{
		class ZModelRunner
		{
		private:
			std::shared_ptr<ZModel> zModel;
			std::shared_ptr<UConverter> uConverter;
			std::vector<std::shared_ptr<ReliabilityResult>> reliabilityResults;
			std::vector<std::shared_ptr<Evaluation>> evaluations;
			std::vector< std::shared_ptr<Message>> messages;
			ProgressIndicator* progressIndicator = nullptr;

			std::shared_ptr<Sample> getXSample(std::shared_ptr<Sample> sample);
			void registerEvaluation(std::shared_ptr<Sample> sample);

		public:
			ZModelRunner(std::shared_ptr<ZModel> zModel, std::shared_ptr<UConverter>uConverter, ProgressIndicator* progressIndicator = nullptr)
			{
				this->zModel = zModel;
				this->uConverter = uConverter;
				this->progressIndicator = progressIndicator;
			}

			void initializeForRun();
			void clear();
			void updateStochastSettings(std::shared_ptr<Reliability::StochastSettingsSet> settings);
			double getZValue(std::shared_ptr<Sample> sample);
			double* getZValues(std::vector<std::shared_ptr<Sample>> samples);
			int getStochastCount();
			int getVaryingStochastCount();
			bool shouldExitPrematurely(double* zValues, double z0Fac, std::vector<std::shared_ptr<Sample>> samples, double beta);
			bool shouldExitPrematurely(bool final);
			void reportResult(ReliabilityReport* report);
			void reportMessage(MessageType type, std::string text);
			std::shared_ptr<DesignPoint> getDesignPoint(double beta, std::vector<double> alpha, std::shared_ptr<ConvergenceReport> convergenceReport = nullptr, int scenarioIndex = -1, std::string identifier = "");

			std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();
		};
	}
}

