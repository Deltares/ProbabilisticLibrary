#pragma once

#include <memory>
#include "../Utils/DirtySupport.h"

namespace Deltares
{
	namespace Statistics
	{
		class HistogramValue
		{
		public:
            HistogramValue() {}

            HistogramValue(double lowerBound, double upperBound)
		    {
                this->LowerBound = lowerBound;
                this->UpperBound = upperBound;
            }

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
			bool contains(double x) const 
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

			bool compareTo(std::shared_ptr<HistogramValue> other)
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

            /**
             * \brief Indicates whether the properties of the histogram value are valid
             */
            bool isValid() const 
			{
				return UpperBound >= LowerBound;
			}

            void setDirtyFunction(Utils::SetDirtyLambda setDirtyLambda)
            {
                this->setDirtyLambda = setDirtyLambda;
            }

            void setDirty()
            {
                if (setDirtyLambda != nullptr)
                {
                    setDirtyLambda();
                }
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
	}
}

