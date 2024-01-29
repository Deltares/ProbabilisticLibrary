#pragma once
namespace Deltares
{
	namespace Statistics
	{
		class DiscreteValue
		{
		public:
			double X;
			double Amount;

			double NormalizedAmount; // for internal use
			double CumulativeNormalizedAmount; // for internal use
		};
	}
}

