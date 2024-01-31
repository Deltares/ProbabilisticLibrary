#pragma once
#include "../Statistics/CorrelationMatrixWrapper.h"
#include "ProgressIndicatorWrapper.h"
#include "RunSettingsWrapper.h"
#include "../Statistics/StochastWrapper.h"
#include "SampleWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Model/ZModel.h"
#include "../../Deltares.Probabilistic.Kernels/Model/ZModelRunner.h"
#include "../../Deltares.Probabilistic.Kernels/Model/Sample.h"
#include "../../Deltares.Probabilistic.Kernels/Utils/SharedPointerProvider.h"

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
				Utils::SharedPointerProvider<Models::ZModelRunner>* sharedPointer = new Utils::SharedPointerProvider<Models::ZModelRunner>();

				std::shared_ptr<Models::ZModel> getZModel();
				Models::ZDelegate getZDelegate();
				Models::ZMultipleDelegate getZMultipleDelegate();
				ZSampleDelegate^ zFunction = nullptr;

				void CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				void CalcZValue(SampleWrapper^ sample);

				void invokeSample(std::shared_ptr<Sample> sample);
				void invokeMultipleSamples(std::vector<std::shared_ptr<Sample>> samples);

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
					delete sharedPointer;
				}

				System::Collections::Generic::List<StochastWrapper^>^ Stochasts = gcnew System::Collections::Generic::List<StochastWrapper^>();

				virtual void GetZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples);
				virtual void GetZValue(SampleWrapper^ sample);

				RunSettingsWrapper^ Settings = gcnew RunSettingsWrapper();

				std::shared_ptr<Models::ZModelRunner> GetModelRunner()
				{
					std::shared_ptr<Models::ZModelRunner> m_modelRunner = sharedPointer->getSharedPointer(modelRunner);

					m_modelRunner->Settings = this->Settings->GetSettings();
					return m_modelRunner;
				}
			};
		}
	}
}




