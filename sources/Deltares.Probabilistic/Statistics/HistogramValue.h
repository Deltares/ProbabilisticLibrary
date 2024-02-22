#pragma once
namespace Deltares
{
	namespace Statistics
	{
		class HistogramValue
		{
		public:
			double LowerBound = 0;
			double UpperBound = 0;
			double Amount = 0;

			double NormalizedAmount = 0;
			double CumulativeNormalizedAmount = 0;

			/// <summary>
			/// Average of lower and upper bound
			/// </summary>
			double getCenter()
			{
				return (LowerBound + UpperBound) / 2;
			}

			/// <summary>
			/// Difference between lower and upper bound
			/// </summary>
			double getSize()
			{
				return UpperBound - LowerBound;
			}

			/// <summary>
			/// Tells whether a value is part of this range
			/// </summary>
			/// <param name="x"></param>
			/// <returns></returns>
			bool contains(double x)
			{
				if (LowerBound == UpperBound)
				{
					return x == LowerBound;
				}
				else
				{
					return x >= LowerBound && x < UpperBound;
				}
			}

			bool CompareTo(HistogramValue* other)
			{
				if (this->LowerBound == other->LowerBound)
				{
					return this->getSize() > other->getSize();
				}
				else
				{
					return this->LowerBound > other->LowerBound;
				}
			}

			bool isValid()
			{
				return UpperBound >= LowerBound;
			}
		};
	}
}

