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

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public enum class WrapperDistributionType {Deterministic, Normal, LogNormal, Uniform, Gumbel, Discrete, Qualitative};

			public enum class WrapperConstantParameterType { Deviation, VariationCoefficient };

			public ref class Stochast
			{
			private:
				Statistics::Stochast* m_stochast;
				SharedPointerProvider<Statistics::Stochast>* sharedPointer = new SharedPointerProvider<Statistics::Stochast>();

				System::Collections::Generic::List<DiscreteValue^>^ discreteValues = gcnew System::Collections::Generic::List<DiscreteValue^>();
				System::Collections::Generic::List<HistogramValue^>^ histogramValues = gcnew System::Collections::Generic::List<HistogramValue^>();
				System::Collections::Generic::List<FragilityValue^>^ fragilityValues = gcnew System::Collections::Generic::List<FragilityValue^>();

				Stochast^ source = nullptr;
				VariableStochastValueSet^ valueSet = gcnew VariableStochastValueSet();

				Statistics::DistributionType getNativeDistributionType(WrapperDistributionType distributionType);
				WrapperDistributionType getManagedDistributionType(Statistics::DistributionType distributionType);
				Statistics::ConstantParameterType getNativeConstantParameterType(WrapperConstantParameterType constantParameterType);

				void updateStochast();

			public:
				Stochast()
				{
					m_stochast = new Statistics::Stochast();
					m_stochast->ValueSet = this->ValueSet->GetValue();
				}
				Stochast(WrapperDistributionType distributionType, array<double>^ values)
				{
					const Statistics::DistributionType nativeDistributionType = getNativeDistributionType(distributionType);
					std::vector<double> nValues = NativeSupport::toNative(values);
					m_stochast = new Statistics::Stochast(nativeDistributionType, nValues);
					m_stochast->ValueSet = this->ValueSet->GetValue();
				}
				~Stochast() { this->!Stochast(); }
				!Stochast() { delete sharedPointer; }

				property WrapperDistributionType DistributionType
				{
					WrapperDistributionType get() { return getManagedDistributionType(m_stochast->getDistributionType()); }
					void set(WrapperDistributionType value) { m_stochast->setDistributionType(getNativeDistributionType(value)); }
				}

				property bool IsVariableStochast
				{
					bool get() { return m_stochast->IsVariableStochast; }
					void set(bool value) { m_stochast->IsVariableStochast = value; }
				}

				property double Mean
				{
					double get() { return m_stochast->getMean(); }
					void set(double value) { m_stochast->setMean(value); }
				}

				property double Deviation
				{
					double get() { return m_stochast->getDeviation(); }
					void set(double value) { m_stochast->setDeviation(value); }
				}

				property double Location
				{
					double get() { return m_stochast->Location; }
					void set(double value) { m_stochast->Location = value; }
				}

				property double Scale
				{
					double get() { return m_stochast->Scale; }
					void set(double value) { m_stochast->Scale = value; }
				}

				property double Shift
				{
					double get() { return m_stochast->Shift; }
					void set(double value) { m_stochast->Shift = value; }
				}

				property double ShiftB
				{
					double get() { return m_stochast->ShiftB; }
					void set(double value) { m_stochast->ShiftB = value; }
				}

				property double Shape
				{
					double get() { return m_stochast->Shape; }
					void set(double value) { m_stochast->Shape = value; }
				}

				property double ShapeB
				{
					double get() { return m_stochast->ShapeB; }
					void set(double value) { m_stochast->ShapeB = value; }
				}

				property double Minimum
				{
					double get() { return m_stochast->Minimum; }
					void set(double value) { m_stochast->Minimum = value; }
				}

				property double Maximum
				{
					double get() { return m_stochast->Maximum; }
					void set(double value) { m_stochast->Maximum = value; }
				}

				property int Observations
				{
					int get() { return m_stochast->Observations; }
					void set(int value) { m_stochast->Observations = value; }
				}

				property bool Truncated
				{
					bool get() { return m_stochast->isTruncated(); }
					void set(bool value) { m_stochast->setTruncated(value); }
				}

				property bool Inverted
				{
					bool get() { return m_stochast->isInverted(); }
					void set(bool value) { m_stochast->setInverted(value); }
				}

				property System::Collections::Generic::List<DiscreteValue^>^ DiscreteValues
				{
					System::Collections::Generic::List<DiscreteValue^>^ get() { return discreteValues; }
				}

				property System::Collections::Generic::List<HistogramValue^>^ HistogramValues
				{
					System::Collections::Generic::List<HistogramValue^>^ get() { return histogramValues; }
				}

				property System::Collections::Generic::List<FragilityValue^>^ FragilityValues
				{
					System::Collections::Generic::List<FragilityValue^>^ get() { return fragilityValues; }
				}

				double GetPDF(double x)
				{
					return m_stochast->getPDF(x);
				}

				double GetCDF(double x)
				{
					return m_stochast->getCDF(x);
				}

				double GetXFromU(double u)
				{
					return m_stochast->getXFromU(u);
				}

				double GetXFromUAndSource(double xSource, double u)
				{
					return m_stochast->getXFromUAndSource(xSource, u);
				}

				double GetXFromP(double p)
				{
					double u = Statistics::StandardNormal::getUFromP(p);
					return m_stochast->getXFromU(u);
				}

				double GetUFromX(double x)
				{
					return m_stochast->getUFromX(x);
				}

				void SetXAtU(double x, double u, WrapperConstantParameterType constantType)
				{
					m_stochast->setXAtU(x, u, this->getNativeConstantParameterType(constantType));
				}

				void InitializeForRun()
				{
					updateStochast();

					m_stochast->initializeForRun();
				}

				void Fit(array<double>^ values)
				{
					std::vector<double> nativaValues = NativeSupport::toNative(values);

					m_stochast->fit(nativaValues);
				}

				property VariableStochastValueSet^ ValueSet
				{
					VariableStochastValueSet^ get() { return valueSet; }
					void set (VariableStochastValueSet^ value) { this->valueSet = value; }
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

