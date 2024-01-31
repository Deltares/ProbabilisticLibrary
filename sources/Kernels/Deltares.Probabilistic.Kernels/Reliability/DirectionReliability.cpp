#include "DirectionReliability.h"
#include "DirectionReliabilitySettings.h"
#include "../Model/ZModelRunner.h"
#include "../Math/RootFinders/LinearRootFinder.h"
#include <memory>

namespace Deltares
{
	namespace Reliability
	{
		class DirectionSection
		{
		public:
			DirectionSection(DoubleType type, double uLow, double uHigh)
			{
				this->Type = type;
				this->ULow = uLow;
				this->UHigh = uHigh;
			}

			DoubleType Type;
			double ULow = 0.0;
			double UHigh = 0.0;

			double getProbability()
			{
				double pHigh = Statistics::StandardNormal::getQFromU(UHigh);
				double pLow = Statistics::StandardNormal::getQFromU(ULow);
				return pLow - pHigh;
			}
		};

		class ZGetter
		{
		private:
			std::shared_ptr<Models::ZModelRunner> modelRunner;
		public:
			ZGetter(std::shared_ptr<Models::ZModelRunner> modelRunner)
			{
				this->modelRunner = modelRunner;
			}

			double GetZ(std::shared_ptr<Sample> uDirection, double factor, bool inverted, bool allowProxy = true)
			{
				std::shared_ptr<Sample> u = uDirection->multiply(factor);
				u->AllowProxy = allowProxy;
				u->ScenarioIndex = uDirection->ScenarioIndex;

				return GetZValueCorrected(u, inverted);
			}

			double GetZValueCorrected(std::shared_ptr<Sample> u, bool invertZ)
			{
				double z = modelRunner->getZValue(u);
				if (invertZ)
				{
					z = -z;
				}
				return z;
			}
		};

		class DirectionCalculation
		{
		private:
			std::shared_ptr<Models::ZModelRunner> modelRunner;
			std::shared_ptr<Sample> uDirection;
			bool inverted;
			ZGetter* model;

		public:
			DirectionCalculation(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> uDirection, bool inverted)
			{
				this->modelRunner = modelRunner;
				this->uDirection = uDirection;
				this->inverted = inverted;
				this->model = new ZGetter(modelRunner);
			}

			double GetZProxy(double u, bool allowProxy)
			{
				return model->GetZ(uDirection, u, inverted, allowProxy);
			}

			double GetZ(double u)
			{
				return model->GetZ(uDirection, u, inverted, true);
			}

			double GetZNoProxy(double u)
			{
				return model->GetZ(uDirection, u, inverted, false);
			}
		};

		std::shared_ptr<DesignPoint> DirectionReliability::getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
			double z = modelRunner->getZValue(zeroSample);
			double z0 = this->getZFactor(z);

			std::shared_ptr<Sample> directionSample = this->Settings->StochastSet->getSample();

			double beta = getBeta(modelRunner, directionSample, z0);
			auto alphas = getAlphas(directionSample, directionSample->getSize(), z0);

			std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(beta, alphas);

			return designPoint;
		}

		double DirectionReliability::getBeta(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> directionSample, double z0)
		{
			std::shared_ptr<Sample> normalizedSample = directionSample->normalize();

			BetaValueTask* task = new BetaValueTask();
			task->ModelRunner = modelRunner;
			task->Index = 0;
			task->Settings = this->Settings;
			task->UValues = normalizedSample;
			task->Iteration = 0;
			task->z0 = z0;

			double beta = this->getDirectionBeta(modelRunner, task);
			beta = beta * z0;

			return beta;
		}

		double DirectionReliability::getDirectionBeta(std::shared_ptr<Models::ZModelRunner> modelRunner, BetaValueTask* directionTask)
		{
			std::shared_ptr<Sample> uDirection = directionTask->UValues->normalize();
			uDirection->IterationIndex = directionTask->Iteration;
			bool invertZ = directionTask->z0 < 0;

			std::vector<std::shared_ptr<DirectionSection>> sections = this->getDirectionSections(modelRunner, directionTask->Settings, uDirection, invertZ);

			double beta = getBetaFromSections(sections);

			directionTask->UValues->AllowProxy = uDirection->AllowProxy;

			return beta;
		}

		std::vector<std::shared_ptr<DirectionSection>> DirectionReliability::getDirectionSections(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ)
		{
			std::vector<std::shared_ptr<DirectionSection>> sections;

			int sectionsCount = static_cast<int>(settings->MaximumLengthU / settings->Dsdu);
			if (sectionsCount * settings->Dsdu < settings->MaximumLengthU)
			{
				sectionsCount = sectionsCount + 1;
			}

			bool found = false;
			bool monotone = settings->modelVaryingType == ModelVaryingType::Monotone;
			std::unique_ptr<ZGetter> model (new ZGetter(modelRunner));

			double prevzHigh = nan("");

			for (int k = 0; k < sectionsCount && !this->isStopped(); k++)
			{
				if (!found)
				{
					double uLow = k * settings->Dsdu;
					double uHigh = std::min((k + 1) * settings->Dsdu, settings->MaximumLengthU);

					double zLow = k > 0 ? prevzHigh : model->GetZ(uDirection, uLow, invertZ);
					double zHigh = model->GetZ(uDirection, uHigh, invertZ);

					prevzHigh = zHigh;

					double zTolerance = GetZTolerance(settings, uLow, uHigh, zLow, zHigh);

					DoubleType zHighType = NumericSupport::getDoubleType(zHigh);
					DoubleType zLowType = NumericSupport::getDoubleType(zLow);

					if (zHighType != zLowType)
					{
						double zChange = nan("");
						double uChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uHigh, zLow, zHigh, zChange);

						DoubleType zChangeType = NumericSupport::getDoubleType(zChange);

						if (zLowType == DoubleType::NaN && zChangeType != zHighType)
						{
							sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uChange));

							double uSubChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uChange, uHigh, zChange, zHigh, zChange);

							sections.push_back(std::make_shared<DirectionSection>(zChangeType, uChange, uSubChange));
							sections.push_back(std::make_shared<DirectionSection>(zHighType, uSubChange, uHigh));
						}
						else if (zHighType == DoubleType::NaN && zChangeType != zLowType)
						{
							double uSubChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uChange, zLow, zChange, zChange);

							sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uSubChange));
							sections.push_back(std::make_shared<DirectionSection>(zChangeType, uSubChange, uChange));
							sections.push_back(std::make_shared<DirectionSection>(zHighType, uChange, uHigh));
						}
						else
						{
							sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uChange));
							sections.push_back(std::make_shared<DirectionSection>(zHighType, uChange, uHigh));
						}

						if (monotone)
						{
							found = true;
						}
					}
					else
					{
						std::shared_ptr<DirectionSection> tempVar9 = std::make_shared<DirectionSection>(zHighType, uLow, uHigh);
						sections.push_back(tempVar9);

						if (monotone && zLowType != DoubleType::NaN && zHighType != DoubleType::NaN)
						{
							if (std::abs(zHigh) > std::abs(zLow))
							{
								found = true;
							}
						}
					}
				}
			}

			// add the remainder to the last 
			std::shared_ptr<DirectionSection> lastSection = sections.back();

			DoubleType lastSectionType = lastSection->Type;

			// when the last type is zero, assume that the line has crossed zero
			if (lastSection->Type == DoubleType::Zero && sections.size() >= 2)
			{
				DoubleType prevSectionType = sections[sections.size() - 2]->Type;
				if (prevSectionType == DoubleType::Positive)
				{
					lastSectionType = DoubleType::Negative;
				}
				else if (prevSectionType == DoubleType::Negative)
				{
					lastSectionType = DoubleType::Positive;
				}
			}

			sections.push_back(std::make_shared<DirectionSection>(lastSectionType, lastSection->UHigh, Statistics::StandardNormal::BetaMax));

			return sections;
		}

		double DirectionReliability::findBetaBetweenBoundaries(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z)
		{
			std::unique_ptr<ZGetter> model (new ZGetter(modelRunner));

			if (std::isnan(zLow) || std::isnan(zHigh))
			{
				double step = uHigh - uLow;
				bool lastAdaptedLow = false;

				while (step > settings->EpsilonUStepSize)
				{
					step = step / 2;

					if (std::isnan(zLow))
					{
						uLow += step;
						zLow = model->GetZ(uDirection, uLow, invertZ);
						lastAdaptedLow = true;
					}
					else if (std::isnan(zHigh))
					{
						uHigh -= step;
						zHigh = model->GetZ(uDirection, uHigh, invertZ);
						lastAdaptedLow = false;
					}
					else if (lastAdaptedLow)
					{
						uHigh = uLow;
						zHigh = zLow;
						uLow -= step;
						zLow = model->GetZ(uDirection, uLow, invertZ);
					}
					else
					{
						uLow = uHigh;
						zLow = zHigh;
						uHigh += step;
						zHigh = model->GetZ(uDirection, uHigh, invertZ);
					}
				}

				if (std::isnan(zLow))
				{
					z = zHigh;
					return uHigh;
				}
				else if (std::isnan(zHigh))
				{
					z = zLow;
					return uLow;
				}
				else
				{
					double u = (uLow + uHigh) / 2;
					z = model->GetZ(uDirection, u, invertZ);

					return u;
				}
			}
			else
			{
				DirectionCalculation* directionCalculation = new DirectionCalculation(modelRunner, uDirection, invertZ);

				double zTolerance = GetZTolerance(settings, uLow, uHigh, zLow, zHigh);

				LinearRootFinder* linearSearchCalculation = new LinearRootFinder();

				double uResult = linearSearchCalculation->CalculateValue(uLow, uHigh, 0, zTolerance, settings->MaximumIterations, [directionCalculation](double v) { return directionCalculation->GetZ(v); }, zLow, zHigh);

				z = std::isnan(uResult) ? nan("") : directionCalculation->GetZ(uResult);

				delete linearSearchCalculation;
				delete directionCalculation;

				return uResult;
			}
		}

		double DirectionReliability::getBetaFromSections(std::vector<std::shared_ptr<DirectionSection>> sections)
		{
			// sum the probabilities
			double failingProbability = 0;
			double nonFailingProbability = 0.5; // start counting at u = 0

			for (int i = sections.size() - 1; i >= 0; i--)
			{
				switch (sections[i]->Type)
				{
				case DoubleType::Positive:
					nonFailingProbability += sections[i]->getProbability();
					break;
				case DoubleType::Negative:
					failingProbability += sections[i]->getProbability();
					break;
				default:
					//nothing to do
					break;
				}
			}

			if (failingProbability == 0 && nonFailingProbability == 0.5)
			{
				return nan("");
			}
			else
			{
				double probFailure = failingProbability / (failingProbability + nonFailingProbability);
				return Statistics::StandardNormal::getUFromQ(probFailure);
			}
		}


		double DirectionReliability::GetZTolerance(std::shared_ptr<DirectionReliabilitySettings> settings, double uLow, double uHigh, double zLow, double zHigh)
		{
			double uDiff = std::abs((zHigh - zLow) / (uHigh - uLow)) * settings->EpsilonUStepSize;

			if (!std::isnan(uDiff))
			{
				return std::min(settings->EpsilonZStepSize, uDiff);
			}
			else
			{
				return settings->EpsilonZStepSize;
			}
		}
	}
}


