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
				Statistics::Stochast* m_stochast;
				Utils::Wrappers::SharedPointerProvider<Statistics::Stochast>* sharedPointer = new Utils::Wrappers::SharedPointerProvider<Statistics::Stochast>();

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
					m_stochast = new Statistics::Stochast();
					m_stochast->ValueSet = this->ValueSet->GetValue();
				}
				Stochast(DistributionType distributionType, array<double>^ values)
				{
					const Statistics::DistributionType nativeDistributionType = DistributionTypeConverter::getNativeDistributionType(distributionType);
					std::vector<double> nValues = NativeSupport::toNative(values);
					m_stochast = new Statistics::Stochast(nativeDistributionType, nValues);
					m_stochast->ValueSet = this->ValueSet->GetValue();
				}
				~Stochast() { this->!Stochast(); }
				!Stochast() { delete sharedPointer; }

				virtual property Wrappers::DistributionType DistributionType
				{
					Wrappers::DistributionType get() { return DistributionTypeConverter::getManagedDistributionType(m_stochast->getDistributionType()); }
					void set(Wrappers::DistributionType value) { m_stochast->setDistributionType(DistributionTypeConverter::getNativeDistributionType(value)); }
				}

				/**
				 * \brief Sets the distribution type and maintains the mean and deviation for some distributions
				 * \param distributionType
				 */
				void SetDistributionType(Wrappers::DistributionType distributionType)
				{
					m_stochast->setDistributionType(DistributionTypeConverter::getNativeDistributionType(distributionType));
				}

				virtual bool IsValid()
				{
					return m_stochast->isValid();
				}

				virtual bool IsVarying()
				{
					return m_stochast->isVarying();
				}

				virtual property bool IsVariableStochast
				{
					bool get() { return m_stochast->IsVariableStochast; }
					void set(bool value) { m_stochast->IsVariableStochast = value; }
				}

				virtual property double Mean
				{
					double get() { return m_stochast->getMean(); }
					void set(double value) { m_stochast->setMean(value); }
				}

				virtual property double Deviation
				{
					double get() { return m_stochast->getDeviation(); }
					void set(double value) { m_stochast->setDeviation(value); }
				}

				virtual property double Location
				{
					double get() { return m_stochast->getProperties()->Location; }
					void set(double value) { m_stochast->getProperties()->Location = value; }
				}

				virtual property double Scale
				{
					double get() { return m_stochast->getProperties()->Scale; }
					void set(double value) { m_stochast->getProperties()->Scale = value; }
				}

				virtual property double Shift
				{
					double get() { return m_stochast->getProperties()->Shift; }
					void set(double value) { m_stochast->getProperties()->Shift = value; }
				}

				void SetShift(double shift)
				{
					m_stochast->setShift(shift);
				}

				virtual property double ShiftB
				{
					double get() { return m_stochast->getProperties()->ShiftB; }
					void set(double value) { m_stochast->getProperties()->ShiftB = value; }
				}

				virtual property double Shape
				{
					double get() { return m_stochast->getProperties()->Shape; }
					void set(double value) { m_stochast->getProperties()->Shape = value; }
				}

				virtual property double ShapeB
				{
					double get() { return m_stochast->getProperties()->ShapeB; }
					void set(double value) { m_stochast->getProperties()->ShapeB = value; }
				}

				virtual property double Minimum
				{
					double get() { return m_stochast->getProperties()->Minimum; }
					void set(double value) { m_stochast->getProperties()->Minimum = value; }
				}

				virtual property double Maximum
				{
					double get() { return m_stochast->getProperties()->Maximum; }
					void set(double value) { m_stochast->getProperties()->Maximum = value; }
				}

				property int Observations
				{
					int get() { return m_stochast->getProperties()->Observations; }
					void set(int value) { m_stochast->getProperties()->Observations = value; }
				}

				virtual bool CanTruncate()
				{
					return m_stochast->canTruncate();
				}

				virtual property bool Truncated
				{
					bool get() { return m_stochast->isTruncated(); }
					void set(bool value) { m_stochast->setTruncated(value); }
				}

				virtual bool CanInvert()
				{
					return m_stochast->canInvert();
				}

				virtual property bool Inverted
				{
					bool get() { return m_stochast->isInverted(); }
					void set(bool value) { m_stochast->setInverted(value); }
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
					return m_stochast->getPDF(x);
				}

				virtual double GetCDF(double x)
				{
					return m_stochast->getCDF(x);
				}

				virtual double GetXFromU(double u)
				{
					return m_stochast->getXFromU(u);
				}

				virtual double GetXFromUAndSource(double xSource, double u)
				{
					return m_stochast->getXFromUAndSource(xSource, u);
				}

				virtual double GetXFromP(double p)
				{
					double u = Statistics::StandardNormal::getUFromP(p);
					return m_stochast->getXFromU(u);
				}

				virtual double GetUFromX(double x)
				{
					return m_stochast->getUFromX(x);
				}

				virtual void SetXAtU(double x, double u, ConstantParameterType constantType)
				{
					m_stochast->setXAtU(x, u, this->getNativeConstantParameterType(constantType));
				}

				virtual void InitializeForRun()
				{
					updateStochast();

					m_stochast->initializeForRun();
				}

				virtual bool CanFit()
				{
					return m_stochast->canFit();
				}

				virtual void Fit(array<double>^ values)
				{
					std::vector<double> nativaValues = NativeSupport::toNative(values);

					m_stochast->fit(nativaValues);
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
						m_stochast->ValueSet = this->ValueSet->GetValue();

						if (this->VariableSource != nullptr)
						{
							m_stochast->VariableSource = this->VariableSource->GetStochast();
						}
					}

					return sharedPointer->getSharedPointer(m_stochast);
				}
			};
		}
	}
}

