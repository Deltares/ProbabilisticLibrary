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
		Sample* StartPointCalculator::getStartPoint(Models::ZModelRunner* modelRunner)
		{
			switch (this->Settings->StartMethod)
			{
			case StartMethodType::None:
				return this->Settings->StochastSet->getSample();
			case StartMethodType::RaySearch:
				return getRayStartPoint(modelRunner);
			case StartMethodType::SensitivitySearch:
				return getSensitivityStartPoint(modelRunner);
			case StartMethodType::SphereSearch:
				return getSphereStartPoint(modelRunner);
			default:
				throw probLibException("Not supported");
			}
		}

		void StartPointCalculator::correctDefaultValues(Sample* startPoint)
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

		Sample* StartPointCalculator::getRayStartPoint(Models::ZModelRunner* modelRunner)
		{
			Sample* startPoint = this->Settings->StochastSet->getSample();

			correctDefaultValues(startPoint);

			Sample* rayStartPoint = getDirectionStartPoint(modelRunner, startPoint);

			delete startPoint;

			return rayStartPoint;
		}

		Sample* StartPointCalculator::getDirectionStartPoint(Models::ZModelRunner* modelRunner, Sample* startPoint)
		{
			int nStochasts = modelRunner->getVaryingStochastCount();

			for (int i = 0; i < nStochasts; i++)
			{
				if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
				{
					startPoint->Values[i] = 0;
				}
			}

			DirectionReliability* directionReliability = new DirectionReliability();
			directionReliability->Settings->StochastSet = this->Settings->StochastSet;
			directionReliability->Settings->MaximumLengthU = this->Settings->MaximumLengthStartPoint;
			directionReliability->Settings->StochastSet = this->Settings->StochastSet;

			double beta = directionReliability->getBeta(modelRunner, startPoint, 1);

			Sample* directionPoint = new Sample(startPoint->Values);

			directionPoint->setBeta(beta);

			for (int i = 0; i < nStochasts; i++)
			{
				if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
				{
					directionPoint->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->StartValue;
				}
			}

			return directionPoint;
		}


		Sample* StartPointCalculator::getSensitivityStartPoint(Models::ZModelRunner* modelRunner)
		{
			int nStochasts = modelRunner->getVaryingStochastCount();
			Sample* startPoint = new Sample(nStochasts);

			Models::GradientCalculator* gradientCalculator = new Models::GradientCalculator();
			gradientCalculator->Settings->GradientType = Models::GradientType::TwoDirections;
			gradientCalculator->Settings->StepSize = this->Settings->GradientStepSize;

			std::vector<double> gradient = gradientCalculator->getGradient(modelRunner, startPoint);

			Sample* gradientSample = new Sample(gradient);

			Sample* sensitivityStartPoint = getDirectionStartPoint(modelRunner, gradientSample);

			delete startPoint;
			delete gradientSample;
			delete gradientCalculator;

			return sensitivityStartPoint;
		}

		Sample* StartPointCalculator::getSphereStartPoint(Models::ZModelRunner* modelRunner)
		{
			constexpr int nRadiusFactors = 20;

			Sample* zeroSample = new Sample(modelRunner->getVaryingStochastCount());
			double z0 = modelRunner->getZValue(zeroSample);
			delete zeroSample;

			double z0Fac = z0 < 0 ? -1 : 1;

			Sample* startPoint = this->Settings->StochastSet->getSample();

			correctDefaultValues(startPoint);

			double radiusFactor = this->Settings->RadiusSphereSearch / startPoint->getBeta();

			Sample* uSphere = startPoint->multiply(radiusFactor);

			Sample* bestSample = nullptr;

			for (int i = 0; i < nRadiusFactors; i++)
			{
				radiusFactor = NumericSupport::Divide(i + 1, nRadiusFactors);

				double zMin = std::numeric_limits<double>::infinity();

				Sample* sample = examineSurfaceForFailure(modelRunner, 0, radiusFactor, uSphere, z0Fac, zMin);

				bestSample = getBestSample(bestSample, sample, z0Fac);

				if (z0Fac * sample->Z < 0)
				{
					Sample* refinedSample = refineSpherePoint(modelRunner, radiusFactor, sample);

					delete sample;

					return refinedSample;
				}
			}

			return bestSample;
		}

		Sample* StartPointCalculator::examineSurfaceForFailure(Models::ZModelRunner* modelRunner, int index, double radiusFactor, Sample* uRay, double z0Fac, double& zMin)
		{
			constexpr int maxSteps = 5;
			constexpr double dangle = 2 * std::numbers::pi / (maxSteps - 1);

			if (index < uRay->getSize())
			{
				int jMax = uRay->Values[index] == 0 ? 1 : maxSteps;

				Sample* bestSample = nullptr;

				for (int j = 0; j < jMax; j++)
				{
					double angle = dangle * j;

					Sample* u = uRay->clone();

					for (int k = 0; k < index; k++)
					{
						u->Values[k] = uRay->Values[k] * std::sin(angle);
					}

					u->Values[index] = uRay->Values[index] * std::cos(angle);

					u->correctSmallValues(1E-10);

					Sample* sample = examineSurfaceForFailure(modelRunner, index + 1, radiusFactor, u, z0Fac, zMin);

					delete u;

					bestSample = getBestSample(bestSample, sample, z0Fac);
				}

				return bestSample;

			}
			else
			{
				Sample* u = uRay->multiply(radiusFactor);

				u->Z = modelRunner->getZValue(u);

				return u;
			}
		}

		// Gets the best sample for a given radius factor

		Sample* StartPointCalculator::getBestSample(Sample* bestSample, Sample* sample, double z0Fac)
		{
			if (bestSample == nullptr)
			{
				return sample;
			}
			else if (z0Fac * bestSample->Z > 0 && z0Fac * sample->Z < 0)
			{
				delete bestSample;
				return sample;
			}
			else if (abs(sample->Z) < abs(bestSample->Z))
			{
				delete bestSample;
				return sample;
			}
			else
			{
				return bestSample;
			}
		}

		Sample* StartPointCalculator::refineSpherePoint(Models::ZModelRunner* modelRunner, double radiusFactor, Sample* u)
		{
			// determine the u-vector for which the z-function is either minimal
			// or where it becomes negative
			// in the latter case, interpolate to get an optimal starting vector

			double z = u->Z;
			double coFactor = (radiusFactor - 0.05) / radiusFactor;

			Sample* u2 = u->multiply(coFactor);

			// factor related to number of steps above
			double z2 = modelRunner->getZValue(u2);
			z2 = std::min(z2, 0.0);

			Sample* u3 = u2->multiply(1.0 + z2 / (z2 - z) / coFactor);

			delete u2;

			return u3;
		}
	}
}


