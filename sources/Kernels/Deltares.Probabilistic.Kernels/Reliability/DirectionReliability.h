#pragma once
#include "DirectionReliabilitySettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class BetaValueTask
		{
		public:
			std::shared_ptr<Models::ModelRunner> ModelRunner;
			double BetaValue;
			int Index = 0;
			int Iteration = 0;
			std::shared_ptr<Sample> UValues;
			std::shared_ptr<DirectionReliabilitySettings> Settings;
			double z0 = 0.0;
			bool IsModelResult = false;
		};

		class DirectionSection;

		class DirectionReliability : public Deltares::Reliability::ReliabilityMethod
		{
		private:
			double getDirectionBeta(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr <BetaValueTask> directionTask);
			std::vector<std::shared_ptr< DirectionSection>> getDirectionSections(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ);
			double findBetaBetweenBoundaries(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z);
			double getBetaFromSections(std::vector<std::shared_ptr<DirectionSection>> sections);
		protected:
			static double GetZTolerance(std::shared_ptr<DirectionReliabilitySettings> settings, double uLow, double uHigh, double zLow, double zHigh);

		public:
			std::shared_ptr<DirectionReliabilitySettings> Settings = std::make_shared<DirectionReliabilitySettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
			double getBeta(std::shared_ptr<Deltares::Models::ModelRunner> modelRunner, std::shared_ptr<Sample> directionSample, double z0);
		};
	}
}

