#pragma once
#include "CorrelationMatrixWrapper.h"
#include "ProgressIndicatorWrapper.h"
#include "RunSettingsWrapper.h"
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
			public delegate void ZSampleDelegate(SampleWrapper^);

			public ref class ModelRunnerWrapper
			{
			private:
				Models::ZModelRunner* modelRunner;
				Models::ZModel* getZModel();
				Models::ZDelegate getZDelegate();
				Models::ZMultipleDelegate getZMultipleDelegate();
				ZSampleDelegate^ zFunction = nullptr;

				void CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				void CalcZValue(SampleWrapper^ sample);

				void invokeSample(Sample* sample);
				void invokeMultipleSamples(Sample** samples, int count);

				System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>^ handles = gcnew System::Collections::Generic::List<System::Runtime::InteropServices::GCHandle>();

			public:
				ModelRunnerWrapper(ZSampleDelegate^ zFunction, System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix, ProgressIndicatorWrapper^ progressIndicator);
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

				RunSettingsWrapper^ Settings = gcnew RunSettingsWrapper();

				Models::ZModelRunner* GetModelRunner()
				{
					modelRunner->Settings = this->Settings->GetSettings();
					return modelRunner;
				}
			};
		}
	}
}




