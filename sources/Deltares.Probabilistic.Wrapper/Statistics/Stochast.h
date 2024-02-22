#pragma once
#include "../../Deltares.Probabilistic/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"
#include "DiscreteValue.h"
#include "HistogramValue.h"
#include "FragilityValue.h"
#include "VariableStochastValueSet.h"
#include "DistributionType.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			public enum class ConstantParameterType { Deviation, VariationCoefficient };

			public ref class Stochast
			{
			private:
				Utils::Wrappers::SharedPointerProvider<Statistics::Stochast>* shared = nullptr;

				System::Collections::Generic::List<DiscreteValue^>^ discreteValues = gcnew System::Collections::Generic::List<DiscreteValue^>();
				System::Collections::Generic::List<HistogramValue^>^ histogramValues = gcnew System::Collections::Generic::List<HistogramValue^>();
				System::Collections::Generic::List<FragilityValue^>^ fragilityValues = gcnew System::Collections::Generic::List<FragilityValue^>();

				Stochast^ source = nullptr;
				VariableStochastValueSet^ valueSet = gcnew VariableStochastValueSet();

				Statistics::ConstantParameterType getNativeConstantParameterType(ConstantParameterType constantParameterType);

				void updateStochast();

			public:
				Stochast()
				{
					shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::Stochast());
					shared->object->ValueSet = this->ValueSet->GetValue();
				}
				Stochast(DistributionType distributionType, array<double>^ values)
				{
					const Statistics::DistributionType nativeDistributionType = DistributionTypeConverter::getNativeDistributionType(distributionType);
					std::vector<double> nValues = NativeSupport::toNative(values);

					shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::Stochast(nativeDistributionType, nValues));
					shared->object->ValueSet = this->ValueSet->GetValue();
				}
				~Stochast() { this->!Stochast(); }
				!Stochast() { delete shared; }

				virtual property Wrappers::DistributionType DistributionType
				{
					Wrappers::DistributionType get() { return DistributionTypeConverter::getManagedDistributionType(shared->object->getDistributionType()); }
					void set(Wrappers::DistributionType value) { shared->object->setDistributionType(DistributionTypeConverter::getNativeDistributionType(value)); }
				}

				/**
				 * \brief Sets the distribution type and maintains the mean and deviation for some distributions
				 * \param distributionType
				 */
				void SetDistributionType(Wrappers::DistributionType distributionType)
				{
					shared->object->setDistributionType(DistributionTypeConverter::getNativeDistributionType(distributionType));
				}

				virtual bool IsValid()
				{
					return shared->object->isValid();
				}

				// TODO: remove this temporary solution when all distributions have been converted
				void SetVarying(bool varying)
				{
					shared->object->SetVarying(varying);
				}

				virtual bool IsVarying()
				{
					return shared->object->isVarying();
				}

				virtual property bool IsVariableStochast
				{
					bool get() { return shared->object->IsVariableStochast; }
					void set(bool value) { shared->object->IsVariableStochast = value; }
				}

				virtual property double Mean
				{
					double get() { return shared->object->getMean(); }
					void set(double value) { shared->object->setMean(value); }
				}

				virtual property double Deviation
				{
					double get() { return shared->object->getDeviation(); }
					void set(double value) { shared->object->setDeviation(value); }
				}

				virtual property double Location
				{
					double get() { return shared->object->getProperties()->Location; }
					void set(double value) { shared->object->getProperties()->Location = value; }
				}

				virtual property double Scale
				{
					double get() { return shared->object->getProperties()->Scale; }
					void set(double value) { shared->object->getProperties()->Scale = value; }
				}

				virtual property double Shift
				{
					double get() { return shared->object->getProperties()->Shift; }
					void set(double value) { shared->object->getProperties()->Shift = value; }
				}

				void SetShift(double shift)
				{
					shared->object->setShift(shift);
				}

				virtual property double ShiftB
				{
					double get() { return shared->object->getProperties()->ShiftB; }
					void set(double value) { shared->object->getProperties()->ShiftB = value; }
				}

				virtual property double Shape
				{
					double get() { return shared->object->getProperties()->Shape; }
					void set(double value) { shared->object->getProperties()->Shape = value; }
				}

				virtual property double ShapeB
				{
					double get() { return shared->object->getProperties()->ShapeB; }
					void set(double value) { shared->object->getProperties()->ShapeB = value; }
				}

				virtual property double Minimum
				{
					double get() { return shared->object->getProperties()->Minimum; }
					void set(double value) { shared->object->getProperties()->Minimum = value; }
				}

				virtual property double Maximum
				{
					double get() { return shared->object->getProperties()->Maximum; }
					void set(double value) { shared->object->getProperties()->Maximum = value; }
				}

				property int Observations
				{
					int get() { return shared->object->getProperties()->Observations; }
					void set(int value) { shared->object->getProperties()->Observations = value; }
				}

				virtual bool CanTruncate()
				{
					return shared->object->canTruncate();
				}

				virtual property bool Truncated
				{
					bool get() { return shared->object->isTruncated(); }
					void set(bool value) { shared->object->setTruncated(value); }
				}

				virtual bool CanInvert()
				{
					return shared->object->canInvert();
				}

				virtual property bool Inverted
				{
					bool get() { return shared->object->isInverted(); }
					void set(bool value) { shared->object->setInverted(value); }
				}

				property System::Collections::Generic::IList<DiscreteValue^>^ DiscreteValues
				{
					System::Collections::Generic::IList<DiscreteValue^>^ get() { return discreteValues; }
				}

				property System::Collections::Generic::IList<HistogramValue^>^ HistogramValues
				{
					System::Collections::Generic::IList<HistogramValue^>^ get() { return histogramValues; }
				}

				property System::Collections::Generic::IList<FragilityValue^>^ FragilityValues
				{
					System::Collections::Generic::IList<FragilityValue^>^ get() { return fragilityValues; }
				}

				virtual double GetPDF(double x)
				{
					return shared->object->getPDF(x);
				}

				virtual double GetCDF(double x)
				{
					return shared->object->getCDF(x);
				}

				virtual double GetXFromU(double u)
				{
					return shared->object->getXFromU(u);
				}

				virtual double GetXFromUAndSource(double xSource, double u)
				{
					return shared->object->getXFromUAndSource(xSource, u);
				}

				virtual double GetXFromP(double p)
				{
					double u = Statistics::StandardNormal::getUFromP(p);
					return shared->object->getXFromU(u);
				}

				virtual double GetUFromX(double x)
				{
					return shared->object->getUFromX(x);
				}

				virtual void SetXAtU(double x, double u, ConstantParameterType constantType)
				{
					shared->object->setXAtU(x, u, this->getNativeConstantParameterType(constantType));
				}

				virtual void InitializeForRun()
				{
					updateStochast();

					shared->object->initializeForRun();
				}

				virtual bool CanFit()
				{
					return shared->object->canFit();
				}

				virtual void Fit(array<double>^ values)
				{
					std::vector<double> nativeValues = NativeSupport::toNative(values);

					shared->object->fit(nativeValues);
				}

				virtual array<double>^ GetSpecialXValues()
				{
					return NativeSupport::toManaged(shared->object->getSpecialXValues());
				}

				property VariableStochastValueSet^ ValueSet
				{
					VariableStochastValueSet^ get() { return valueSet; }
					void set(VariableStochastValueSet^ value) { this->valueSet = value; }
				}

				property Stochast^ VariableSource
				{
					Stochast^ get() { return this->source; }
					void set(Stochast^ value) { this->source = value; }
				}

				std::shared_ptr<Statistics::Stochast> GetStochast()
				{
					updateStochast();

					if (this->IsVariableStochast)
					{
						shared->object->ValueSet = this->ValueSet->GetValue();

						if (this->VariableSource != nullptr)
						{
							shared->object->VariableSource = this->VariableSource->GetStochast();
						}
					}

					return shared->object;
				}
			};
		}
	}
}

