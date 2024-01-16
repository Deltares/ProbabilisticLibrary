#pragma once


namespace Deltares
{
	namespace Reliability
	{
		enum ModelVaryingType {Monotone, Varying};

		class DirectionReliabilitySettings
		{
		public:
			ModelVaryingType ModelVaryingType = ModelVaryingType::Monotone;
			double MaximumLengthU = 20;
			double Dsdu = 1;
			double EpsilonUStepSize = 0.01;
			int MaximumIterations = 50;
			double EpsilonZStepSize = 0.01;

			bool IsValid();
		};
	}
}

