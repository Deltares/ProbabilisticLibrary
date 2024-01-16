#include "../Model/Sample.h"
#include "../Math/NumericSupport.h"
#include "../Model/ZModelRunner.h"
#include "DesignPointBuilder.h"

#include <vector>
#include <cmath>
#include <map>
#include <stdexcept>

namespace Deltares
{
	namespace Reliability
	{
		class ModeFinder
		{
		private:
			std::map<double, double> values;
			StochastSettings* settings = nullptr;

		public:
			ModeFinder(StochastSettings* settings)
			{
				this->settings = settings;
			}

			void add(double u, double weight)
			{
				u = settings->getRepresentativeU(u);

				if (!values.contains(u))
				{
					values[u] = 0;
				}

				values[u] += weight;
			}

			double getMode()
			{
				double mode = 0;
				double max = -1;

				for (auto it = values.begin(); it != values.end(); ++it)
				{
					if (it->second > max)
					{
						mode = it->first;
						max = it->second;
					}
				}

				return mode;
			}
		};

		DesignPointBuilder::DesignPointBuilder(int count, DesignPointMethod method, StochastSettingsSet* stochastSet)
		{
			this->count = count;
			this->method = method;

			defaultSample = new Sample(count);
			meanSample = new Sample(count);
			sinSample = new Sample(count);
			cosSample = new Sample(count);

			this->qualitativeCount = 0;
			for (int i = 0; i < stochastSet->VaryingStochastCount; i++)
			{
				if (stochastSet->VaryingStochastSettings[i]->IsQualitative)
				{
					this->qualitativeCount++;
				}
			}

			this->qualitativeIndices = new int[this->qualitativeCount];
			this->modeFinders = new ModeFinder * [this->qualitativeCount];

			int j = 0;
			for (int i = 0; i < stochastSet->VaryingStochastCount; i++)
			{
				if (stochastSet->VaryingStochastSettings[i]->IsQualitative)
				{
					this->qualitativeIndices[j] = i;
					this->modeFinders[j] = new ModeFinder(stochastSet->VaryingStochastSettings[i]);
				}
			}
		}

		void DesignPointBuilder::initialize(double beta)
		{
			double value = NumericSupport::GetSign(beta) * sqrt(abs(beta) / count);

			for (int i = 0; i < count; i++)
			{
				defaultSample->Values[i] = value;
			}
		}

		void DesignPointBuilder::addSample(Sample* sample)
		{
			sampleAdded = true;

			switch (method)
			{
			case DesignPointMethod::NearestToMean:
			{
				double rbeta = sample->getBeta();
				if (rbeta < rmin)
				{
					rmin = rbeta;
					meanSample = sample;
				}
				break;
			}
			case DesignPointMethod::CenterOfGravity:
			{
				for (int j = 0; j < this->qualitativeCount; j++)
				{
					int qIndex = qualitativeIndices[j];
					modeFinders[j]->add(sample->Values[qIndex], sample->Weight);
				}

				for (int i = 0; i < sample->getSize(); i++)
				{
					meanSample->Values[i] += sample->Weight * sample->Values[i];
				}

				sumWeights += sample->Weight;
				break;
			}
			case DesignPointMethod::CenterOfAngles:
			{
				for (int j = 0; j < this->qualitativeCount; j++)
				{
					int qIndex = qualitativeIndices[j];
					modeFinders[j]->add(sample->Values[qIndex], sample->Weight);
				}

				double* sphericalValues = NumericSupport::GetSphericalCoordinates(sample->Values, count);
				meanSample->Values[0] += sample->Weight * sphericalValues[0];
				for (int i = 1; i < count; i++)
				{
					sinSample->Values[i] += sample->Weight * std::sin(sphericalValues[i]);
					cosSample->Values[i] += sample->Weight * std::cos(sphericalValues[i]);
				}
				sumWeights += sample->Weight;
				break;
			}
			default:
				throw std::runtime_error("Not supported");
			}
		}

		Sample* DesignPointBuilder::getSample()
		{
			if (!sampleAdded)
			{
				return defaultSample;
			}
			else
			{
				switch (method)
				{
				case DesignPointMethod::NearestToMean:
				{
					return meanSample;
				}
				case DesignPointMethod::CenterOfGravity:
				{
					double* gravityValues = new double[count];

					for (int i = 0; i < count; i++)
					{
						gravityValues[i] = meanSample->Values[i] / sumWeights;
					}

					Sample* gravityPoint = new Sample(gravityValues, count);

					for (int j = 0; j < this->qualitativeCount; j++)
					{
						int qIndex = qualitativeIndices[j];
						gravityPoint->Values[qIndex] = modeFinders[j]->getMode();
					}

					return gravityPoint;
				}
				case DesignPointMethod::CenterOfAngles:
				{
					double* angleValues = new double[count];
					angleValues[0] = meanSample->Values[0] / sumWeights;
					for (int i = 1; i < count; i++)
					{
						angleValues[i] = std::atan2(sinSample->Values[i] / sumWeights, cosSample->Values[i] / sumWeights);
					}
					double* coordinates = NumericSupport::GetCartesianCoordinates(angleValues, count);
					Sample* anglePoint = new Sample(coordinates, count);

					for (int j = 0; j < this->qualitativeCount; j++)
					{
						int qIndex = qualitativeIndices[j];
						anglePoint->Values[qIndex] = modeFinders[j]->getMode();
					}

					return anglePoint;
				}
				default:
					throw std::runtime_error("Not supported");
				}
			}
		}
	}
}



