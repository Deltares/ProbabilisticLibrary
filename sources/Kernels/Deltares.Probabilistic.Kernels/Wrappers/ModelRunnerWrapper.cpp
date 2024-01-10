#include "ModelRunnerWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			ModelRunnerWrapper::ModelRunnerWrapper(System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix, ZSampleDelegate^ zFunction)
			{
				ModelRunnerWrapper::instance = this;

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

				this->modelRunner = new ZModelRunner(zModel, uConverter);
			}

			ZDelegate ModelRunnerWrapper::getZDelegate()
			{
				ZSampleDelegate^ zSampleDelegate = gcnew ZSampleDelegate(this->CalcZValue);
				ModelRunnerWrapper::zSampleFunction = zSampleDelegate;

				ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(ModelRunnerWrapper::invokeSample);
				System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				ZDelegate functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			ZMultipleDelegate ModelRunnerWrapper::getZMultipleDelegate()
			{
				ZMultipleSampleDelegate^ zSampleDelegate = gcnew ZMultipleSampleDelegate(this->CalcZValues);
				ModelRunnerWrapper::zMultipleSampleFunction = zSampleDelegate;

				ManagedMultipleSampleDelegate^ fp = gcnew ManagedMultipleSampleDelegate(ModelRunnerWrapper::invokeMultipleSamples);
				System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

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

				zSampleFunction->Invoke(sampleWrapper);
			}

			void ModelRunnerWrapper::invokeMultipleSamples(Sample** samples, int count)
			{
				System::Collections::Generic::List<SampleWrapper^>^ sampleWrappers = gcnew System::Collections::Generic::List<SampleWrapper^>();

				for (int i = 0; i < count; i++)
				{
					sampleWrappers->Add(gcnew SampleWrapper(samples[i]));
				}

				zMultipleSampleFunction->Invoke(sampleWrappers);
			}

			void ModelRunnerWrapper::CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples)
			{
				instance->GetZValues(samples);
			}

			void ModelRunnerWrapper::CalcZValue(SampleWrapper^ sample)
			{
				instance->GetZValue(sample);
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
				instance->zFunction->Invoke(sample);
			}
		}
	}
}

