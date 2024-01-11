#pragma once
#include "CorrelationMatrixWrapper.h"
#include "StochastWrapper.h"
#include "SampleWrapper.h"
#include "../Model/ZModel.h"
#include "../Model/ZModelRunner.h"
#include "../Model/Sample.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public delegate void ManagedSampleDelegate(Sample* sample);
			public delegate void ManagedMultipleSampleDelegate(Sample** samples, int count);

			public delegate void ZSampleDelegate(SampleWrapper^);
			public delegate void ZMultipleSampleDelegate(System::Collections::Generic::IList<SampleWrapper^>^);


			public ref class ModelRunnerWrapper
			{
			private:
				ZModelRunner* modelRunner;
				ZModel* getZModel();
				ZDelegate getZDelegate();
				ZMultipleDelegate getZMultipleDelegate();
				ZSampleDelegate^ zFunction = nullptr;

				void CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				void CalcZValue(SampleWrapper^ sample);

				ZSampleDelegate^ zSampleFunction;
				ZMultipleSampleDelegate^ zMultipleSampleFunction;

				void invokeSample(Sample* sample);
				void invokeMultipleSamples(Sample** samples, int count);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

			public:
				ModelRunnerWrapper(System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix, ZSampleDelegate^ zFunction);
				~ModelRunnerWrapper() { this->!ModelRunnerWrapper(); }
				!ModelRunnerWrapper()
				{
					for (int i = 0; i < handles->Count; i++)
					{
						handles[i].Free();
					}
					delete modelRunner;
				}

				System::Collections::Generic::List<StochastWrapper^>^ Stochasts = gcnew System::Collections::Generic::List<StochastWrapper^>();

				virtual void GetZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				virtual void GetZValue(SampleWrapper^ sample);

				ZModelRunner* GetModelRunner()
				{
					return modelRunner;
				}
			};
		}
	}
}




