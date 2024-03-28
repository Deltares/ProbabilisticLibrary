#pragma once
namespace Deltares
{
	namespace Statistics
	{
		class DiscreteValue
		{
		public:
			DiscreteValue()
			{
				id = ++counter;
			}

			DiscreteValue(double x, double amount)
			{
				this->X = x;
				this->Amount = amount;

				id = ++counter;
			}

			~DiscreteValue()
			{
			}

			static int inline counter = 0;
			int id = 0;

			double X = 0;
			double Amount = 0;

			double NormalizedAmount = 0; // for internal use
			double CumulativeNormalizedAmount = 0; // for internal use
		};
	}
}

