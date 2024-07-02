#pragma once

#include <cmath>
#include <vector>
#include <cstdint>
#include <memory>

#include "Message.h"
#include "../Math/NumericSupport.h"


namespace Deltares
{
	namespace Models
	{
        class ModelSample
		{
		private:
			long tag = 0;

		public:
			ModelSample(std::vector<double> values)
			{
				this->Values = values;
			}

			std::vector<double> Values;

			int IterationIndex = -1;
			int threadId = 0;
			double Weight = 1;
			bool AllowProxy = true;
			bool IsRestartRequired = false;
			double Beta = 0;
			double Z = nan("");

            bool ExtendedLogging = false;

            /**
             * Indication of a logging message
             */
            int LoggingCounter = 0;


            intptr_t Tag = 0;

			bool hasSameValues(std::shared_ptr<ModelSample> other)
			{
				if (this->Values.size() != other->Values.size())
				{
					return false;
				}

				for (int i = 0; i < this->Values.size(); i++)
				{
					if (this->Values[i] != other->Values[i])
					{
						return false;
					}
				}

				return true;
			}
		};
	}
}

