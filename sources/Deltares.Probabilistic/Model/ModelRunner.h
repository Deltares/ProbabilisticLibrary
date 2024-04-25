#pragma once
#include <string>

#include "Sample.h"
#include <vector>

#include "../Reliability/ReliabilityReport.h"
#include "../Reliability/ReliabilityResult.h"
#include "../Reliability/DesignPoint.h"
#include "../Reliability/StochastSettingsSet.h"
#include "../Utils/Locker.h"
#include "RunSettings.h"
#include "UConverter.h"
#include "ZModel.h"
#include "Message.h"
#include "ModelSample.h"
#include "ProgressIndicator.h"

namespace Deltares
{
	namespace Models
	{
		class ModelRunner
		{
		public:
			ModelRunner(std::shared_ptr<ZModel> zModel, std::shared_ptr<UConverter>uConverter, std::shared_ptr<ProgressIndicator> progressIndicator = nullptr)
			{
				this->zModel = zModel;
				this->uConverter = uConverter;
				this->progressIndicator = progressIndicator;
			}

			~ModelRunner()
			{
				delete this->locker;
			}

			void initializeForRun();
			void clear();
			void updateStochastSettings(std::shared_ptr<Reliability::StochastSettingsSet> settings);
			double getZValue(std::shared_ptr<Sample> sample);
			std::vector<double> getZValues(std::vector<std::shared_ptr<Sample>> samples);
			int getStochastCount();
			int getVaryingStochastCount();
			bool shouldExitPrematurely(std::vector<std::shared_ptr<Sample>> samples);
			void reportResult(std::shared_ptr<Reliability::ReliabilityReport> report);
			void reportMessage(MessageType type, std::string text);
			void doTextualProgress(ProgressType type, std::string text);
			bool isVaryingStochast(int index);
			std::shared_ptr<Reliability::DesignPoint> getDesignPoint(std::shared_ptr<Sample> sample, double beta, std::shared_ptr<Reliability::ConvergenceReport> convergenceReport = nullptr, std::string identifier = "");
			std::shared_ptr<RunSettings> Settings = std::make_shared<RunSettings>();
			std::shared_ptr<Models::ModelSample> getModelSample(std::shared_ptr<Sample> sample);
			std::vector<double> getOnlyVaryingValues(std::vector<double> values);
		private:
			std::shared_ptr<ZModel> zModel;
			std::shared_ptr<UConverter> uConverter;
			std::vector<std::shared_ptr<Reliability::ReliabilityResult>> reliabilityResults;
			std::vector<std::shared_ptr<Evaluation>> evaluations;
			std::vector< std::shared_ptr<Message>> messages;
			std::shared_ptr<ProgressIndicator> progressIndicator = nullptr;

			void registerEvaluation(std::shared_ptr<ModelSample> sample);

			Utils::Locker* locker = nullptr;
		};
	}
}

