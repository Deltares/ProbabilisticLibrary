#pragma once
namespace Deltares
{
	namespace Statistics
	{
		class HistogramValue
		{
		public:
			double LowerBound = 0;
			double UpperBound = 0;;
			double Amount = 0;
		private:
			double NormalizedAmount = 0;
		};
	}
}

