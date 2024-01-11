#include "ModelRunnerWrapper.h"

#include "ProgressIndicatorWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			delegate void ManagedSampleDelegate(Sample* sample);
			delegate void ManagedMultipleSampleDelegate(Sample** samples, int count);

			ModelRunnerWrapper::ModelRunnerWrapper(ZSampleDelegate^ zFunction, System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix, ProgressIndicatorWrapper^ progressIndicator)
			{
				this->zFunction = zFunction;

				this->Stochasts->AddRange(stochasts);

				std::vector<Stochast*> native_stochasts;

				for (int i = 0; i < stochasts->Count; i++)
				{
					native_stochasts.push_back(stochasts[i]->GetStochast());
				}

				UConverter* uConverter = new UConverter(native_stochasts, correlationMatrix->GetCorrelationMatrix());
				uConverter->initializeForRun();

				ZModel* zModel = getZModel();

				Models::ProgressIndicator* progress = progressIndicator != nullptr ? progressIndicator->getProgressIndicator() : nullptr;

				this->modelRunner = new Models::ZModelRunner(zModel, uConverter, progress);
			}

			ZDelegate ModelRunnerWrapper::getZDelegate()
			{
				ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(this, &ModelRunnerWrapper::invokeSample);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				ZDelegate functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			ZMultipleDelegate ModelRunnerWrapper::getZMultipleDelegate()
			{
				ManagedMultipleSampleDelegate^ fp = gcnew ManagedMultipleSampleDelegate(this, &ModelRunnerWrapper::invokeMultipleSamples);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				ZMultipleDelegate functionPointer = static_cast<ZMultipleDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			ZModel* ModelRunnerWrapper::getZModel()
			{
				ZModel* zModel = new ZModel();
				zModel->setZDelegate(getZDelegate());
				zModel->setZMultipleDelegate(getZMultipleDelegate());

				return zModel;
			}

			void ModelRunnerWrapper::invokeSample(Sample* sample)
			{
				SampleWrapper^ sampleWrapper = gcnew SampleWrapper(sample);

				this->CalcZValue(sampleWrapper);
			}

			void ModelRunnerWrapper::invokeMultipleSamples(Sample** samples, int count)
			{
				System::Collections::Generic::List<SampleWrapper^>^ sampleWrappers = gcnew System::Collections::Generic::List<SampleWrapper^>();

				for (int i = 0; i < count; i++)
				{
					sampleWrappers->Add(gcnew SampleWrapper(samples[i]));
				}

				this->CalcZValues(sampleWrappers);
			}

			void ModelRunnerWrapper::CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples)
			{
				this->GetZValues(samples);
			}

			void ModelRunnerWrapper::CalcZValue(SampleWrapper^ sample)
			{
				this->GetZValue(sample);
			}

			void ModelRunnerWrapper::GetZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples)
			{
				for (int i = 0; i < samples->Count; i++)
				{
					GetZValue(samples[i]);
				}
			}

			void ModelRunnerWrapper::GetZValue(SampleWrapper^ sample)
			{
				this->zFunction->Invoke(sample);
			}
		}
	}
}

