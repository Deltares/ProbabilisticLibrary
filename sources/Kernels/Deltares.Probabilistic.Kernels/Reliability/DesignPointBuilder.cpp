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
			std::shared_ptr<StochastSettings> settings = nullptr;

		public:
			ModeFinder(std::shared_ptr<StochastSettings> settings)
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

		DesignPointBuilder::DesignPointBuilder(int count, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet)
		{
			this->count = count;
			this->method = method;

			defaultSample = std::make_shared<Sample>(count);
			meanSample = std::make_shared<Sample>(count);
			sinSample = std::make_shared<Sample>(count);
			cosSample = std::make_shared<Sample>(count);

			this->qualitativeIndices.clear();
			this->modeFinders.clear();

			for (int i = 0; i < stochastSet->getVaryingStochastCount(); i++)
			{
				if (stochastSet->VaryingStochastSettings[i]->IsQualitative)
				{
					this->qualitativeIndices.push_back(i);
					this->modeFinders.push_back(std::make_shared<ModeFinder>(stochastSet->VaryingStochastSettings[i]));
				}
			}

			this->qualitativeCount = this->qualitativeIndices.size();
		}

		void DesignPointBuilder::initialize(double beta)
		{
			double value = NumericSupport::GetSign(beta) * sqrt(abs(beta) / count);

			for (int i = 0; i < count; i++)
			{
				defaultSample->Values[i] = value;
			}
		}

		void DesignPointBuilder::addSample(std::shared_ptr<Sample> sample)
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

					meanSample = sample->clone();
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

				auto sphericalValues = NumericSupport::GetSphericalCoordinates(sample->Values);
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

		std::shared_ptr<Sample> DesignPointBuilder::getSample()
		{
			if (!sampleAdded)
			{
				return defaultSample->clone();
			}
			else
			{
				switch (method)
				{
				case DesignPointMethod::NearestToMean:
				{
					return meanSample->clone();
				}
				case DesignPointMethod::CenterOfGravity:
				{
					std::shared_ptr<Sample> gravityPoint = std::make_shared<Sample>(count);

					for (int i = 0; i < count; i++)
					{
						gravityPoint->Values[i] = meanSample->Values[i] / sumWeights;
					}

					for (int j = 0; j < this->qualitativeCount; j++)
					{
						int qIndex = qualitativeIndices[j];
						gravityPoint->Values[qIndex] = modeFinders[j]->getMode();
					}

					return gravityPoint;
				}
				case DesignPointMethod::CenterOfAngles:
				{
					auto angleValues = std::vector<double>(count);
					angleValues[0] = meanSample->Values[0] / sumWeights;
					for (int i = 1; i < count; i++)
					{
						angleValues[i] = std::atan2(sinSample->Values[i] / sumWeights, cosSample->Values[i] / sumWeights);
					}

					auto coordinates = NumericSupport::GetCartesianCoordinates(angleValues);
					std::shared_ptr<Sample> anglePoint = std::make_shared<Sample>(coordinates);

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



