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
			public enum class DistributionType { Deterministic, Normal, LogNormal, Uniform, Triangular, Trapezoidal, Exponential, Gamma, Beta, Frechet, Weibull, Gumbel, GeneralizedExtremeValue, StudentT, Rayleigh, Pareto, GeneralizedPareto, Table, CDFCurve, Discrete, Bernoulli, Poisson, Composite, X, Qualitative };

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

				Statistics::DistributionType getNativeDistributionType(DistributionType distributionType);
				DistributionType getManagedDistributionType(Statistics::DistributionType distributionType);
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
					const Statistics::DistributionType nativeDistributionType = getNativeDistributionType(distributionType);
					std::vector<double> nValues = NativeSupport::toNative(values);
					m_stochast = new Statistics::Stochast(nativeDistributionType, nValues);
					m_stochast->ValueSet = this->ValueSet->GetValue();
				}
				~Stochast() { this->!Stochast(); }
				!Stochast() { delete sharedPointer; }

				virtual property Wrappers::DistributionType DistributionType
				{
					Wrappers::DistributionType get() { return getManagedDistributionType(m_stochast->getDistributionType()); }
					void set(Wrappers::DistributionType value) { m_stochast->setDistributionType(getNativeDistributionType(value)); }
				}

				/**
				 * \brief Sets the distribution type while maintaining the mean and deviation
				 * \param distributionType 
				 */
				void SetDistributionType(Wrappers::DistributionType distributionType)
				{
					double oldMean = m_stochast->getMean();
					double oldDeviation = m_stochast->getDistributionType() == Deterministic ? m_stochast->Scale : m_stochast->getDeviation();

					m_stochast->setDistributionType(getNativeDistributionType(distributionType));

					if (oldMean != 0 || oldDeviation != 0) 
					{
						m_stochast->setMeanAndDeviation(oldMean, oldDeviation);
					}
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
					double get() { return m_stochast->Location; }
					void set(double value) { m_stochast->Location = value; }
				}

				virtual property double Scale
				{
					double get() { return m_stochast->Scale; }
					void set(double value) { m_stochast->Scale = value; }
				}

				virtual property double Shift
				{
					double get() { return m_stochast->Shift; }
					void set(double value) { m_stochast->Shift = value; }
				}

				void SetShift(double shift)
				{
					m_stochast->setShift(shift);
				}

				virtual property double ShiftB
				{
					double get() { return m_stochast->ShiftB; }
					void set(double value) { m_stochast->ShiftB = value; }
				}

				virtual property double Shape
				{
					double get() { return m_stochast->Shape; }
					void set(double value) { m_stochast->Shape = value; }
				}

				virtual property double ShapeB
				{
					double get() { return m_stochast->ShapeB; }
					void set(double value) { m_stochast->ShapeB = value; }
				}

				virtual property double Minimum
				{
					double get() { return m_stochast->Minimum; }
					void set(double value) { m_stochast->Minimum = value; }
				}

				virtual property double Maximum
				{
					double get() { return m_stochast->Maximum; }
					void set(double value) { m_stochast->Maximum = value; }
				}

				property int Observations
				{
					int get() { return m_stochast->Observations; }
					void set(int value) { m_stochast->Observations = value; }
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

