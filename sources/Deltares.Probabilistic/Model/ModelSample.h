#pragma once

#include <cmath>
#include <vector>
#include <memory>

namespace Deltares
{
	namespace Models
	{
        class ModelSample
		{
		public:
			ModelSample(std::vector<double> values)
			{
				this->Values = values;
			}

            /**
             * \brief Resets all contents of the sample to its default values
             * \remark Values are not cleared, since they are provided in the constructor
             */
            void clear()
            {
                IterationIndex = -1;
                threadId = 0;
                Weight = std::nan("");
                AllowProxy = true;
                IsRestartRequired = false;
                Z = std::nan("");
                Beta = 0;
                Tag = 0;
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


            bool ExtendedLogging = false;

            /**
             * Indication of a logging message
             */
            int LoggingCounter = 0;


			int Tag = 0;

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

