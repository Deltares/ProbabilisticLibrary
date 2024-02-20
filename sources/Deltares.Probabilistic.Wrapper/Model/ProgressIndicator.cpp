#include "ProgressIndicator.h"
#include "../../Deltares.Probabilistic/Model/ProgressIndicator.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public delegate void ManagedProgressDelegate(double progress);
			public delegate void ManagedDetailedProgressDelegate(int step, int loop, double reliability, double convergence);
			public delegate void ManagedTextualProgressDelegate(Models::ProgressType, std::string text);

			ProgressIndicator::ProgressIndicator(ProgressDelegate^ progressDelegate, DetailedProgressDelegate^ detailedProgressDelegate, TextualProgressDelegate^ textualProgressDelegate)
			{
				this->progressDelegate = progressDelegate;
				this->detailedProgressDelegate = detailedProgressDelegate;
				this->textualProgressDelegate = textualProgressDelegate;

				Deltares::Models::ProgressLambda modelProgressLambda = getProgressLambda();
				Deltares::Models::DetailedProgressLambda modelDetailedProgressLambda = getDetailedProgressLambda();
				Deltares::Models::TextualProgressLambda modelTextualProgressLambda = getTextualProgressLambda();

				this->shared = new Utils::Wrappers::SharedPointerProvider<Deltares::Models::ProgressIndicator>(new Models::ProgressIndicator(modelProgressLambda, modelDetailedProgressLambda, modelTextualProgressLambda));
			}

			void ProgressIndicator::DoProgress(double progress)
			{
				if (this->progressDelegate != nullptr) 
				{
					this->progressDelegate->Invoke(progress);
				}
			}

			void ProgressIndicator::DoDetailedProgress(int step, int loop, double reliability, double convergence)
			{
				if (this->detailedProgressDelegate != nullptr)
				{
					this->detailedProgressDelegate->Invoke(step, loop, reliability, convergence);
				}
			}

			void ProgressIndicator::DoTextualProgress(Models::ProgressType progressType, std::string text)
			{
				if (this->textualProgressDelegate != nullptr)
				{
					this->textualProgressDelegate->Invoke(GetProgressType(progressType), gcnew System::String(text.c_str()));
				}
			}

			void ProgressIndicator::DoTextualProgress(Wrappers::ProgressType progressType, System::String^ text)
			{
				if (this->textualProgressDelegate != nullptr)
				{
					this->textualProgressDelegate->Invoke(progressType, text);
				}
			}

			ProgressType ProgressIndicator::GetProgressType(Models::ProgressType progressType)
			{
				switch (progressType)
				{
				case Models::ProgressType::Detailed: return Wrappers::ProgressType::Detailed;
				case Models::ProgressType::Global: return Wrappers::ProgressType::Global;
				default: throw gcnew System::Exception("Progress type not supported");
				}
			}

			Models::ProgressLambda ProgressIndicator::getProgressLambda()
			{
				ManagedProgressDelegate^ fp = gcnew ManagedProgressDelegate(this, &ProgressIndicator::DoProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::ProgressLambda functionPointer = static_cast<Models::ProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			Models::DetailedProgressLambda ProgressIndicator::getDetailedProgressLambda()
			{
				ManagedDetailedProgressDelegate^ fp = gcnew ManagedDetailedProgressDelegate(this, &ProgressIndicator::DoDetailedProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::DetailedProgressLambda functionPointer = static_cast<Models::DetailedProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}

			Models::TextualProgressLambda ProgressIndicator::getTextualProgressLambda()
			{
				ManagedTextualProgressDelegate^ fp = gcnew ManagedTextualProgressDelegate(this, &ProgressIndicator::DoTextualProgress);
				System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(fp);
				handles->Add(handle);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				Models::TextualProgressLambda functionPointer = static_cast<Models::TextualProgressDelegate>(callbackPtr.ToPointer());

				return functionPointer;
			}
		}
	}
}

