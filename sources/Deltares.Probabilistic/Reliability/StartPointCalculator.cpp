#include "StartPointCalculator.h"

#include "DirectionReliability.h"
#include "../Utils/probLibException.h"
#include "../Model/GradientCalculator.h"

#include <numbers>
#include <limits>

using namespace Deltares::ProbLibCore;

namespace Deltares
{
	namespace Reliability
	{
		std::shared_ptr<Sample> StartPointCalculator::getStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			switch (this->Settings->StartMethod)
			{
			case StartMethodType::None:
				return this->Settings->StochastSet->getSample();
			case StartMethodType::One:
				return getOneStartPoint(modelRunner);
			case StartMethodType::RaySearch:
				return getRayStartPoint(modelRunner);
			case StartMethodType::SensitivitySearch:
				return getSensitivityStartPoint(modelRunner);
			case StartMethodType::SphereSearch:
				return getSphereStartPoint(modelRunner);
			case StartMethodType::GivenVector:
				return getGivenVectorStartPoint(modelRunner);
			default:
				throw probLibException("Start method not supported: ", (int)this->Settings->StartMethod);
			}
		}

		void StartPointCalculator::correctDefaultValues(std::shared_ptr<Sample> startPoint)
		{
			bool isDefaultStartValues = true;
			for (int i = 0; i < startPoint->getSize(); i++)
			{
				if (this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
				{
					isDefaultStartValues &= startPoint->Values[i] == 0;
				}
			}

			if (isDefaultStartValues)
			{
				for (int i = 0; i < startPoint->getSize(); i++)
				{
					startPoint->Values[i] = 1;
				}
			}
		}

		std::shared_ptr<Sample> StartPointCalculator::getOneStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getSample();

			correctDefaultValues(startPoint);

			return startPoint;
		}

		std::shared_ptr<Sample> StartPointCalculator::getGivenVectorStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getSample();
			startPoint->Values = this->Settings->startVector;
			return startPoint;
		}

		std::shared_ptr<Sample> StartPointCalculator::getRayStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getSample();

			if (this->Settings->startVector.size() > 0)
			{
				for (int i = 0; i < startPoint.get()->getSize(); i++)
				{
					startPoint->Values[i] = this->Settings->startVector[i];
				}
			}
			else
			{
				correctDefaultValues(startPoint);
			}

			std::shared_ptr<Sample> rayStartPoint = getDirectionStartPoint(modelRunner, startPoint);

			return rayStartPoint;
		}

		std::shared_ptr<Sample> StartPointCalculator::getDirectionStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startPoint)
		{
			int nStochasts = modelRunner->getVaryingStochastCount();

			for (int i = 0; i < nStochasts; i++)
			{
				if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
				{
					startPoint->Values[i] = 0;
				}
			}

			std::unique_ptr<DirectionReliability> directionReliability(new DirectionReliability());
			directionReliability->Settings->StochastSet = this->Settings->StochastSet;
			directionReliability->Settings->MaximumLengthU = this->Settings->MaximumLengthStartPoint;
			directionReliability->Settings->StochastSet = this->Settings->StochastSet;
			directionReliability->Settings->FindMinimalValue = true;
			directionReliability->Settings->UseInitialValues = true;
			directionReliability->Settings->modelVaryingType = Varying;
			directionReliability->Settings->Dsdu = this->Settings->dsdu;

			double beta = directionReliability->getBeta(modelRunner, startPoint, 1);

			std::shared_ptr<Sample> directionPoint = std::make_shared<Sample>(startPoint->Values);

			directionPoint = directionPoint->getSampleAtBeta(std::min(beta, this->Settings->MaximumLengthStartPoint));

			for (int i = 0; i < nStochasts; i++)
			{
				if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
				{
					directionPoint->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->StartValue;
				}
			}

			return directionPoint;
		}

		std::shared_ptr<Sample> StartPointCalculator::getSensitivityStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			int nStochasts = modelRunner->getVaryingStochastCount();
			std::shared_ptr<Sample> startPoint = std::make_shared<Sample>(nStochasts);

			std::vector<double> gradient = this->getGradient(modelRunner, startPoint);

			std::shared_ptr<Sample> gradientSample = std::make_shared<Sample>(gradient);

			std::shared_ptr<Sample> sensitivityStartPoint = getDirectionStartPoint(modelRunner, gradientSample);

			return sensitivityStartPoint;
		}

		std::vector<double> StartPointCalculator::getGradient(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample)
		{
			int nstochasts = modelRunner->getVaryingStochastCount();

			std::vector<std::shared_ptr<Sample>> samples;
			std::vector<double> gradient(nstochasts);

			double stepSize = 2;

			// first sample is the sample itself
			samples.push_back(sample);

			for (int k = 0; k < nstochasts; k++)
			{
				std::shared_ptr<Sample> u1 = sample->clone();
				u1->Values[k] -= stepSize;
				samples.push_back(u1);

				std::shared_ptr<Sample> u2 = sample->clone();
				u2->Values[k] += stepSize;
				samples.push_back(u2);
			}

			std::vector<double> zValues = modelRunner->getZValues(samples);

			double z0Fac = Numeric::NumericSupport::GetSign(zValues[0]);
			double z0 = zValues[0] * z0Fac;

			for (int k = 0; k < nstochasts; k++)
			{
				const double zLow = zValues[2 * k + 1] * z0Fac;
				const double zHigh = zValues[2 * k + 2] * z0Fac;

				if (zLow < z0)
				{
					gradient[k] = 1.0 / Numeric::NumericSupport::interpolate(0, z0, 0, zLow, -stepSize, true);
				}
				else if (zHigh < z0)
				{
					gradient[k] = 1.0 / Numeric::NumericSupport::interpolate(0, z0, 0, zHigh, stepSize, true);
				}
				else
				{
					gradient[k] = 0;
				}
			}

			return gradient;
		}

		std::shared_ptr<Sample> StartPointCalculator::getSphereStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
		{
			constexpr int nRadiusFactors = 20;

			std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
			double z0 = modelRunner->getZValue(zeroSample);

			double z0Fac = z0 < 0 ? -1 : 1;

			std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getSample();

			correctDefaultValues(startPoint);

			double radiusFactor = this->Settings->RadiusSphereSearch / startPoint->getBeta();

			std::shared_ptr<Sample> uSphere = startPoint->getMultipliedSample(radiusFactor);

			std::shared_ptr<Sample> bestSample = nullptr;

			for (int i = 0; i < nRadiusFactors; i++)
			{
				radiusFactor = Numeric::NumericSupport::Divide(i + 1, nRadiusFactors);

				double zMin = std::numeric_limits<double>::infinity();

				std::shared_ptr<Sample> sample = examineSurfaceForFailure(modelRunner, 0, radiusFactor, uSphere, z0Fac, zMin);

				bestSample = getBestSample(bestSample, sample, z0Fac);

				if (z0Fac * sample->Z < 0)
				{
					std::shared_ptr<Sample> refinedSample = refineSpherePoint(modelRunner, radiusFactor, sample);

					return refinedSample;
				}
			}

			return bestSample;
		}

		std::shared_ptr<Sample> StartPointCalculator::examineSurfaceForFailure(std::shared_ptr<Models::ModelRunner> modelRunner, int index, double radiusFactor, std::shared_ptr<Sample> uRay, double z0Fac, double& zMin)
		{
			constexpr int maxSteps = 5;
			constexpr double dangle = 2 * std::numbers::pi / (maxSteps - 1);

			if (index < uRay->getSize())
			{
				int jMax = uRay->Values[index] == 0 ? 1 : maxSteps;

				std::shared_ptr<Sample> bestSample = nullptr;

				for (int j = 0; j < jMax; j++)
				{
					double angle = dangle * j;

					std::shared_ptr<Sample> u = uRay->clone();

					for (int k = 0; k < index; k++)
					{
						u->Values[k] = uRay->Values[k] * std::sin(angle);
					}

					u->Values[index] = uRay->Values[index] * std::cos(angle);

					u->correctSmallValues(1E-10);

					std::shared_ptr<Sample> sample = examineSurfaceForFailure(modelRunner, index + 1, radiusFactor, u, z0Fac, zMin);

					bestSample = getBestSample(bestSample, sample, z0Fac);
				}

				return bestSample;

			}
			else
			{
				std::shared_ptr<Sample> u = uRay->getMultipliedSample(radiusFactor);

				u->Z = modelRunner->getZValue(u);

				return u;
			}
		}

		// Gets the best sample for a given radius factor

		std::shared_ptr<Sample> StartPointCalculator::getBestSample(std::shared_ptr<Sample> bestSample, std::shared_ptr<Sample> sample, double z0Fac)
		{
			if (bestSample == nullptr)
			{
				return sample;
			}
			else if (z0Fac * bestSample->Z > 0 && z0Fac * sample->Z < 0)
			{
				return sample;
			}
			else if (abs(sample->Z) < abs(bestSample->Z))
			{
				return sample;
			}
			else
			{
				return bestSample;
			}
		}

		std::shared_ptr<Sample> StartPointCalculator::refineSpherePoint(std::shared_ptr<Models::ModelRunner> modelRunner, double radiusFactor, std::shared_ptr<Sample> u)
		{
			// determine the u-vector for which the z-function is either minimal
			// or where it becomes negative
			// in the latter case, interpolate to get an optimal starting vector

			double z = u->Z;
			double coFactor = (radiusFactor - 0.05) / radiusFactor;

			std::shared_ptr<Sample> u2 = u->getMultipliedSample(coFactor);

			// factor related to number of steps above
			double z2 = modelRunner->getZValue(u2);

			// assume fz(uFactor) = A + B * uFactor
			// z = A + B * 1.0
			// z2 = A + B * coFactor
			// z-z2 = B(1-coFactor) => B = (z-z2)/(1-coFactor)
			// A = z - B
			// fz = 0 => A + B * uFactor = 0
			// => uFactor = -A / B
			// if B near 0 => z near z2 => refinement does not improve the result, so return u
			double B = (z - z2) / (1.0 - coFactor);
			double A = z - B;

			if (std::abs(B) > 1e-25)
			{
				double uFactor = -A / B;

				std::shared_ptr<Sample> u3 = u->getMultipliedSample(uFactor);
				return u3;
			}
			else
			{
				return u;
			}

		}
	}
}


