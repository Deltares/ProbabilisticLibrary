#pragma once
#include "../../Deltares.Probabilistic.Kernels/Statistics/DistributionType.h"
#include "../../Deltares.Probabilistic.Kernels/Statistics/Stochast.h"
#include "../../Deltares.Probabilistic.Kernels/Statistics/StandardNormal.h"
#include "../Utils/SharedPointerProvider.h"
#include "../Utils/NativeSupport.h"
#include "DiscreteValueWrapper.h"
#include "HistogramValueWrapper.h"
#include "FragilityValueWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public enum class WrapperDistributionType {Deterministic, Normal, LogNormal, Uniform, Gumbel, Discrete, Qualitative};

			public ref class StochastWrapper
			{
			private:
				Statistics::Stochast* m_stochast;
				SharedPointerProvider<Statistics::Stochast>* sharedPointer = new SharedPointerProvider<Statistics::Stochast>();

				System::Collections::Generic::List<DiscreteValueWrapper^>^ discreteValues = gcnew System::Collections::Generic::List<DiscreteValueWrapper^>();
				System::Collections::Generic::List<HistogramValueWrapper^>^ histogramValues = gcnew System::Collections::Generic::List<HistogramValueWrapper^>();
				System::Collections::Generic::List<FragilityValueWrapper^>^ fragilityValues = gcnew System::Collections::Generic::List<FragilityValueWrapper^>();

				Statistics::DistributionType getNativeDistributionType(WrapperDistributionType distributionType);
				WrapperDistributionType getManagedDistributionType(Statistics::DistributionType distributionType);

				void updateStochast();

			public:
				StochastWrapper() { m_stochast = new Statistics::Stochast(); }
				StochastWrapper(WrapperDistributionType distributionType, array<double>^ values)
				{
					const Statistics::DistributionType nativeDistributionType = getNativeDistributionType(distributionType);
					double* nValues = NativeSupport::toNative(values);
					m_stochast = new Statistics::Stochast(nativeDistributionType, nValues);
				}
				~StochastWrapper() { this->!StochastWrapper(); }
				!StochastWrapper() { delete sharedPointer; }

				property WrapperDistributionType DistributionType
				{
					WrapperDistributionType get() { return getManagedDistributionType(m_stochast->getDistributionType()); }
					void set(WrapperDistributionType value) { m_stochast->setDistributionType(getNativeDistributionType(value)); }
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

				property System::Collections::Generic::List<DiscreteValueWrapper^>^ DiscreteValues
				{
					System::Collections::Generic::List<DiscreteValueWrapper^>^ get() { return discreteValues; }
				}

				property System::Collections::Generic::List<HistogramValueWrapper^>^ HistogramValues
				{
					System::Collections::Generic::List<HistogramValueWrapper^>^ get() { return histogramValues; }
				}

				property System::Collections::Generic::List<FragilityValueWrapper^>^ FragilityValues
				{
					System::Collections::Generic::List<FragilityValueWrapper^>^ get() { return fragilityValues; }
				}

				double GetXFromU(double u)
				{
					return m_stochast->getXFromU(u);
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

				void InitializeForRun()
				{
					updateStochast();
					m_stochast->initializeForRun();
				}

				std::shared_ptr<Statistics::Stochast> GetStochast()
				{
					updateStochast();
					return sharedPointer->getSharedPointer(m_stochast);
				}
			};
		}
	}
}

