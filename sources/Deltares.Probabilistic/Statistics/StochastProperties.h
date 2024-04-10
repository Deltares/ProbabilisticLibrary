#pragma once

#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"
#include "../Utils/probLibException.h"

#include <vector>
#include <memory>

namespace Deltares
{
	namespace Statistics
	{
		enum DistributionPropertyType { Location, Scale, Minimum, Maximum, Shift, ShiftB, Shape, ShapeB, Observations };

		class StochastProperties
		{

		public:

			double Location = 0;
			double Scale = 0;
			double Minimum = 0;
			double Maximum = 0;
			double Shift = 0;
			double ShiftB = 0;
			double Shape = 1;
			double ShapeB = 1;
			int Observations = 2;

			std::vector<std::shared_ptr<DiscreteValue>> DiscreteValues;
			std::vector<std::shared_ptr<HistogramValue>> HistogramValues;
			std::vector<std::shared_ptr<FragilityValue>> FragilityValues;

			void applyValue(DistributionPropertyType property, double value)
			{
				switch (property)
				{
					case DistributionPropertyType::Location: this->Location = value; break;
					case DistributionPropertyType::Scale: this->Scale = value; break;
					case DistributionPropertyType::Minimum:	this->Minimum = value; break;
					case DistributionPropertyType::Maximum:	this->Maximum = value; break;
					case DistributionPropertyType::Shift: this->Shift = value; break;
					case DistributionPropertyType::ShiftB: this->ShiftB = value; break;
					case DistributionPropertyType::Shape: this->Shape = value; break;
					case DistributionPropertyType::ShapeB: this->ShapeB = value; break;
					case DistributionPropertyType::Observations: this->Observations = value; break;
					default: throw Reliability::probLibException("Property not supported");
				}
			}

			std::shared_ptr<StochastProperties> clone()
			{
				std::shared_ptr<StochastProperties> clone = std::make_shared<StochastProperties>();

				clone->Location = this->Location;
				clone->Scale = this->Scale;
				clone->Minimum = this->Minimum;
				clone->Maximum = this->Maximum;
				clone->Shift = this->Shift;
				clone->ShiftB = this->ShiftB;
				clone->Shape = this->Shape;
				clone->ShapeB = this->ShapeB;
				clone->Observations = this->Observations;

				return clone;
			}
		};
	}
}
