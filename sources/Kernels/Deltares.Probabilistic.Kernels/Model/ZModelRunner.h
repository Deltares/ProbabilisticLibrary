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
			ZModel* zModel;
			UConverter* uConverter;
			std::vector<ReliabilityResult*> reliabilityResults;
			std::vector<Evaluation*> evaluations;
			std::vector<Message*> messages;
			ProgressIndicator* progressIndicator = nullptr;

			Sample* getXSample(Sample* sample);
			void registerEvaluation(Sample* sample);

		public:
			ZModelRunner(ZModel* zModel, UConverter* uConverter, ProgressIndicator* progressIndicator = nullptr)
			{
				this->zModel = zModel;
				this->uConverter = uConverter;
				this->progressIndicator = progressIndicator;
			}

			void initializeForRun();
			void clear();
			void updateStochastSettings(Reliability::StochastSettingsSet* settings);
			double getZValue(Sample* sample);
			double* getZValues(std::vector<Sample*> samples);
			int getStochastCount();
			int getVaryingStochastCount();
			bool shouldExitPrematurely(double* zValues, double z0Fac, std::vector<Sample*> samples, double beta);
			bool shouldExitPrematurely(bool final);
			void reportResult(ReliabilityReport* report);
			void reportMessage(MessageType type, std::string text);
			DesignPoint* getDesignPoint(double beta, std::vector<double> alpha, ConvergenceReport* convergenceReport = nullptr, int scenarioIndex = -1, std::string identifier = "");

			RunSettings* Settings = new ::RunSettings();
		};
	}
}

