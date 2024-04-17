#include "StartPointCalculator.h"

#include "DirectionReliability.h"
#include "../Utils/probLibException.h"
#include "../Model/GradientCalculator.h"
#include "SphereTasks.h"

#include <numbers>
#include <limits>

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
			constexpr int maxSteps = 5;

			std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
			double z0 = modelRunner->getZValue(zeroSample);

			double z0Fac = z0 < 0 ? -1 : 1;

			std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getSample();

			correctDefaultValues(startPoint);

			double radiusFactor = this->Settings->RadiusSphereSearch / startPoint->getBeta();

			std::shared_ptr<Sample> uSphere = startPoint->getMultipliedSample(radiusFactor);

			if (this->Settings->startVector.size() > 0)
			{
				for (size_t i = 0; i < this->Settings->startVector.size(); i++)
				{
					uSphere->Values[i] = this->Settings->startVector[i];
				}
			}

			auto st = sphereTasks(maxSteps, Settings->allQuadrants);
			auto uSphereValues = vector1D(uSphere->Values.size());
			for (size_t i = 0; i < uSphere->Values.size(); i++)
			{
				uSphereValues(i) = uSphere->Values[i];
			}
			auto tasks = st.examineSurfaceForTasks(uSphereValues);

			std::shared_ptr<Sample> bestSample = nullptr;
			std::vector<std::shared_ptr<Sample>> previousSamples;

			for (int i = 0; i < nRadiusFactors; i++)
			{
				radiusFactor = Numeric::NumericSupport::Divide(i + 1, nRadiusFactors);

				std::vector<std::shared_ptr<Sample>> samples;
				for (const auto& task : tasks)
				{
					std::shared_ptr<Sample> uRay = this->Settings->StochastSet->getSample();
					for (int k = 0; k < uSphere->Values.size(); k++)
					{
						uRay->Values[k] = task(k);
					}
					std::shared_ptr<Sample> u = uRay->getMultipliedSample(radiusFactor);
					samples.push_back(u);
				}
				modelRunner->getZValues(samples);
				size_t bestDirection = maxSteps;
				for (size_t i = 0; i < maxSteps; i++)
				{
					if (getBestSample(bestSample, samples[i], z0Fac)) bestDirection = i;
				}

				if (z0Fac * bestSample->Z < 0.0)
				{
					auto previous = (bestDirection < previousSamples.size() ? previousSamples[i] : zeroSample);
					std::shared_ptr<Sample> refinedSample = refineSpherePoint(radiusFactor, bestSample, previous);

					return refinedSample;
				}
				previousSamples = samples;
			}

			return bestSample;
		}

		// Gets the best sample for a given radius factor
		// Best sample defined as direction where z has the first change of sign
		bool StartPointCalculator::getBestSample(std::shared_ptr<Sample> & bestSample, const std::shared_ptr<Sample> sample, const double z0Fac)
		{
			if (bestSample == nullptr)
			{
				bestSample = sample;
				return true;
			}
			else if (z0Fac * bestSample->Z > 0 && z0Fac * sample->Z < 0)
			{
				bestSample = sample;
				return true;
			}
			else if (std::abs(sample->Z) > std::abs(bestSample->Z) && z0Fac * bestSample->Z < 0 && z0Fac * sample->Z < 0)
			{
				bestSample = sample;
				return true;
			}
			else
			{
				return false;
			}
		}

		std::shared_ptr<Sample> StartPointCalculator::refineSpherePoint(const double radiusFactor, const std::shared_ptr<Sample> u,
			const std::shared_ptr<Sample> previous)
		{
			// determine the u-vector for which the z-function is either minimal
			// or where it becomes negative
			// in the latter case, interpolate to get an optimal starting vector

			double z = u->Z;
			double coFactor = (radiusFactor - 0.05) / radiusFactor;

			std::shared_ptr<Sample> u2 = u->getMultipliedSample(coFactor);

			// factor related to number of steps above
			double z2 = previous->Z;

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


