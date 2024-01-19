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
			Models::ZModelRunner* ModelRunner;
			double BetaValue;
			int Index = 0;
			int Iteration = 0;
			Sample* UValues;
			DirectionReliabilitySettings* Settings;
			double z0 = 0.0;
			bool IsModelResult = false;
		};

		class DirectionSection;

		class DirectionReliability : public Deltares::Reliability::ReliabilityMethod
		{
		private:
			double getDirectionBeta(Models::ZModelRunner* modelRunner, BetaValueTask* directionTask);
			std::vector<DirectionSection*> getDirectionSections(Models::ZModelRunner* modelRunner, DirectionReliabilitySettings* settings, Sample* uDirection, bool invertZ);
			double findBetaBetweenBoundaries(Models::ZModelRunner* modelRunner, DirectionReliabilitySettings* settings, Sample* uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z);
			double getBetaFromSections(std::vector<DirectionSection*> sections);
		protected:
			static double GetZTolerance(DirectionReliabilitySettings* settings, double uLow, double uHigh, double zLow, double zHigh);

		public:
			Deltares::Reliability::DirectionReliabilitySettings* Settings = new Deltares::Reliability::DirectionReliabilitySettings();
			DesignPoint* getDesignPoint(Deltares::Models::ZModelRunner* modelRunner) override;
			double getBeta(Deltares::Models::ZModelRunner* modelRunner, Sample* directionSample, double z0);
		};
	}
}

