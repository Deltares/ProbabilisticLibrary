#include "ProgressIndicatorWrapper.h"
#include "../../Deltares.Probabilistic.Kernels/Model/ProgressIndicator.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public delegate void ManagedProgressDelegate(double progress);
			public delegate void ManagedDetailedProgressDelegate(int step, int loop, double reliability, double convergence);
			public delegate void ManagedTextualProgressDelegate(Models::ProgressType, std::string text);

			ProgressIndicatorWrapper::ProgressIndicatorWrapper(ProgressDelegate^ progressDelegate, DetailedProgressDelegate^ detailedProgressDelegate, TextualProgressDelegate^ textualProgressDelegate)
			{
				this->progressDelegate = progressDelegate;
				this->detailedProgressDelegate = detailedProgressDelegate;
				this->textualProgressDelegate = textualProgressDelegate;

				Deltares::Models::ProgressDelegate modelProgressDelegate = getProgressDelegate();
				Deltares::Models::DetailedProgressDelegate modelDetailedProgressDelegate = getDetailedProgressDelegate();
				Deltares::Models::TextualProgressDelegate modelTextualProgressDelegate = getTextualProgressDelegate();

				this->progressIndicator = new Models::ProgressIndicator(modelProgressDelegate, modelDetailedProgressDelegate, modelTextualProgressDelegate);
			}

			void ProgressIndicatorWrapper::doProgress(double progress)
			{
				if (this->progressDelegate != nullptr) 
				{
					this->progressDelegate->Invoke(progress);
				}
			}

			void ProgressIndicatorWrapper::doDetailedProgress(int step, int loop, double reliability, double convergence)
			{
				if (this->detailedProgressDelegate != nullptr)
				{
					this->detailedProgressDelegate->Invoke(step, loop, reliability, convergence);
				}
			}

			ProgressTextType ProgressIndicatorWrapper::getProgressType(Models::ProgressType progressType)
			{
				switch (progressType)
				{
					case Models::ProgressType::Detailed: return ProgressTextType::Detailed;
					case Models::ProgressType::Global: return ProgressTextType::Global;
					default: throw gcnew System::Exception("Progress type not supported");
				}
			}

			void ProgressIndicatorWrapper::doTextualProgress(Models::ProgressType progressType, std::string text)
			{
				if (this->textualProgressDelegate != nullptr)
				{
					this->textualProgressDelegate->Invoke(getProgressType(progressType), gcnew System::String(text.c_str()));
				}
			}

			Models::ProgressDelegate ProgressIndicatorWrapper::getProgressDelegate()
			{
				ManagedProgressDelegate^ fp = gcnew ManagedProgressDelegate(this, &ProgressIndicatorWrapper::doProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::ProgressDelegate functionPointer = static_cast<Models::ProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			Models::DetailedProgressDelegate ProgressIndicatorWrapper::getDetailedProgressDelegate()
			{
				ManagedDetailedProgressDelegate^ fp = gcnew ManagedDetailedProgressDelegate(this, &ProgressIndicatorWrapper::doDetailedProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::DetailedProgressDelegate functionPointer = static_cast<Models::DetailedProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			Models::TextualProgressDelegate ProgressIndicatorWrapper::getTextualProgressDelegate()
			{
				ManagedTextualProgressDelegate^ fp = gcnew ManagedTextualProgressDelegate(this, &ProgressIndicatorWrapper::doTextualProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::TextualProgressDelegate functionPointer = static_cast<Models::TextualProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}
		}
	}
}

