#include "DirectionReliability.h"
#include "DirectionReliabilitySettings.h"
#include "../Model/ZModelRunner.h"
#include "../Math/RootFinders/LinearRootFinder.h"

namespace Deltares
{
	namespace Reliability
	{
		class BetaValueTask
		{
		public:
			Models::ZModelRunner* ModelRunner;
			double BetaValue;
			int Index = 0;
			int Iteration = 0;
			Sample* UValues;
			DirectionReliabilitySettings* Settings;
			double z0 = 0.0;
			bool IsModelResult = false;
		};

		class DirectionSection
		{
		public:
			DoubleType Type;
			double ULow = 0.0;
			double UHigh = 0.0;

			double getProbability()
			{
				double pHigh = StandardNormal::getQFromU(UHigh);
				double pLow = StandardNormal::getQFromU(ULow);
				return pLow - pHigh;
			}
		};

		class ZGetter
		{
		private:
			Models::ZModelRunner* modelRunner;
		public:
			ZGetter(Models::ZModelRunner* modelRunner)
			{
				this->modelRunner = modelRunner;
			}

			double GetZ(Sample* uDirection, double factor, bool inverted, bool allowProxy = true)
			{
				Sample* u = uDirection->multiply(factor);
				u->AllowProxy = allowProxy;
				u->ScenarioIndex = uDirection->ScenarioIndex;

				return GetZValueCorrected(u, inverted);
			}

			double GetZValueCorrected(Sample* u, bool invertZ)
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
			Models::ZModelRunner* modelRunner;
			Sample* uDirection;
			bool inverted;
			ZGetter* model;

		public:
			DirectionCalculation(Models::ZModelRunner* modelRunner, Sample* uDirection, bool inverted)
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

		DesignPoint* DirectionReliability::getDesignPoint(Deltares::Models::ZModelRunner* modelRunner)
		{
			modelRunner->updateStochastSettings(this->Settings->StochastSet);

			Sample* zeroSample = new Sample(modelRunner->getVaryingStochastCount());
			double z = modelRunner->getZValue(zeroSample);
			double z0 = this->getZFactor(z);

			Sample* directionSample = this->Settings->StochastSet->getSample();

			double beta = getBeta(modelRunner, directionSample, z0);
			double* alphas = GetAlphas(directionSample, directionSample->getSize(), z0);

			DesignPoint* designPoint = modelRunner->getRealization(beta, alphas);

			return designPoint;
		}

		double DirectionReliability::getBeta(Deltares::Models::ZModelRunner* modelRunner, Sample* directionSample, double z0)
		{
			Sample* normalizedSample = directionSample->normalize();

			BetaValueTask* task = new BetaValueTask();
			task->ModelRunner = modelRunner;
			task->Index = 0;
			task->Settings = this->Settings;
			task->UValues = normalizedSample;
			task->Iteration = 0;

			double beta = this->GetDirectionBeta(modelRunner, task);

			// direction beta is always positive, therefore correct
			beta = beta * z0;

			return beta;
		}

		double DirectionReliability::GetDirectionBeta(Models::ZModelRunner* modelRunner, BetaValueTask* directionTask)
		{
			Sample* uDirection = directionTask->UValues->normalize();
			uDirection->IterationIndex = directionTask->Iteration;
			bool invertZ = directionTask->z0 < 0;

			std::vector<DirectionSection*> sections = this->GetDirectionSections(modelRunner, directionTask->Settings, uDirection, invertZ);

			double beta = GetBetaFromSections(sections);

			directionTask->UValues->AllowProxy = uDirection->AllowProxy;

			return beta;
		}

		std::vector<DirectionSection*> DirectionReliability::GetDirectionSections(Models::ZModelRunner* modelRunner, DirectionReliabilitySettings* settings, Sample* uDirection, bool invertZ)
		{
			std::vector<DirectionSection*> sections;

			int sectionsCount = static_cast<int>(settings->MaximumLengthU / settings->Dsdu);
			if (sectionsCount * settings->Dsdu < settings->MaximumLengthU)
			{
				sectionsCount = sectionsCount + 1;
			}

			bool found = false;
			bool monotone = settings->ModelVaryingType == ModelVaryingType::Monotone;
			ZGetter* model = new ZGetter(modelRunner);

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
						double uChange = FindBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uHigh, zLow, zHigh, zChange);

						DoubleType zChangeType = NumericSupport::getDoubleType(zChange);

						if (zLowType == DoubleType::NaN && zChangeType != zHighType)
						{
							DirectionSection* tempVar = new DirectionSection();
							tempVar->Type = zLowType;
							tempVar->ULow = uLow;
							tempVar->UHigh = uChange;
							sections.push_back(tempVar);

							double uSubChange = FindBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uChange, uHigh, zChange, zHigh, zChange);

							DirectionSection* tempVar2 = new DirectionSection();
							tempVar2->Type = zChangeType;
							tempVar2->ULow = uChange;
							tempVar2->UHigh = uSubChange;
							sections.push_back(tempVar2);

							DirectionSection* tempVar3 = new DirectionSection();
							tempVar3->Type = zHighType;
							tempVar3->ULow = uSubChange;
							tempVar3->UHigh = uHigh;
							sections.push_back(tempVar3);
						}
						else if (zHighType == DoubleType::NaN && zChangeType != zLowType)
						{
							double uSubChange = FindBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uChange, zLow, zChange, zChange);

							DirectionSection* tempVar4 = new DirectionSection();
							tempVar4->Type = zLowType;
							tempVar4->ULow = uLow;
							tempVar4->UHigh = uSubChange;
							sections.push_back(tempVar4);

							DirectionSection* tempVar5 = new DirectionSection();
							tempVar5->Type = zChangeType;
							tempVar5->ULow = uSubChange;
							tempVar5->UHigh = uChange;
							sections.push_back(tempVar5);

							DirectionSection* tempVar6 = new DirectionSection();
							tempVar6->Type = zHighType;
							tempVar6->ULow = uChange;
							tempVar6->UHigh = uHigh;
							sections.push_back(tempVar6);
						}
						else
						{
							DirectionSection* tempVar7 = new DirectionSection();
							tempVar7->Type = zLowType;
							tempVar7->ULow = uLow;
							tempVar7->UHigh = uChange;
							sections.push_back(tempVar7);

							DirectionSection* tempVar8 = new DirectionSection();
							tempVar8->Type = zHighType;
							tempVar8->ULow = uChange;
							tempVar8->UHigh = uHigh;
							sections.push_back(tempVar8);
						}

						if (monotone)
						{
							found = true;
						}
					}
					else
					{
						DirectionSection* tempVar9 = new DirectionSection();
						tempVar9->Type = zHighType;
						tempVar9->ULow = uLow;
						tempVar9->UHigh = uHigh;
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
			DirectionSection* lastSection = sections.back();

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

			DirectionSection* tempVar10 = new DirectionSection();
			tempVar10->Type = lastSectionType;
			tempVar10->ULow = lastSection->UHigh;
			tempVar10->UHigh = StandardNormal::BetaMax;
			sections.push_back(tempVar10);

			return sections;
		}

		double DirectionReliability::FindBetaBetweenBoundaries(Models::ZModelRunner* modelRunner, DirectionReliabilitySettings* settings, Sample* uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z)
		{
			ZGetter* model = new ZGetter(modelRunner);

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
				return uResult;
			}
		}

		double DirectionReliability::GetBetaFromSections(std::vector<DirectionSection*> sections)
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
				return StandardNormal::getUFromQ(probFailure);
			}
		}


		double DirectionReliability::GetZTolerance(DirectionReliabilitySettings* settings, double uLow, double uHigh, double zLow, double zHigh)
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


