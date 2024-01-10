#include "ModelRunnerWrapper.h"
#include "FunctionWrapper.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			ModelRunnerWrapper::ModelRunnerWrapper(System::Collections::Generic::List<StochastWrapper^>^ stochasts, CorrelationMatrixWrapper^ correlationMatrix)
			{
				ModelRunnerWrapper::instance = this;

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
				FunctionWrapper::setMethod(zSampleDelegate);

				ManagedSampleDelegate^ fp = gcnew ManagedSampleDelegate(FunctionWrapper::invokeSample);
				System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				ZDelegate functionPointer = static_cast<ZDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			ZMultipleDelegate ModelRunnerWrapper::getZMultipleDelegate()
			{
				ZMultipleSampleDelegate^ zSampleDelegate = gcnew ZMultipleSampleDelegate(this->CalcZValues);
				FunctionWrapper::setMethod(zSampleDelegate);

				ManagedMultipleSampleDelegate^ fp = gcnew ManagedMultipleSampleDelegate(FunctionWrapper::invokeMultipleSamples);
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

			void ModelRunnerWrapper::CalcZValues(System::Collections::Generic::IList<SampleWrapper^>^ samples)
			{
				instance->GetZValues(samples);
			}

			void ModelRunnerWrapper::CalcZValue(SampleWrapper^ sample)
			{
				instance->GetZValue(sample);
			}

		}
	}
}

