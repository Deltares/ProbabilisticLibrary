#pragma once
#include <string>
#include <functional>

namespace Deltares
{
	namespace Models
	{
		enum ProgressType { Global, Detailed };

#ifdef _WIN32
		typedef void(__stdcall* ProgressDelegate) (double);
		typedef void(__stdcall* DetailedProgressDelegate) (int, int, double, double);
		typedef void(__stdcall* TextualProgressDelegate) (ProgressType, std::string);
#else
		typedef void(* ProgressDelegate) (double);
		typedef void(* DetailedProgressDelegate) (int, int, double, double);
		typedef void(* TextualProgressDelegate) (ProgressType, std::string);
#endif
		typedef std::function<void(ProgressType, std::string)> TextualProgressLambda;

		class ProgressIndicator
		{
		private:
			ProgressDelegate progressDelegate;
			DetailedProgressDelegate detailedProgressDelegate;
			TextualProgressDelegate textualProgressDelegate;
			TextualProgressLambda textualProgressLambda;

		public:
			ProgressIndicator(ProgressDelegate progressDelegate, DetailedProgressDelegate detailedProgressDelegate = nullptr, TextualProgressDelegate textualProgressDelegate = nullptr)
			{
				this->progressDelegate = progressDelegate;
				this->detailedProgressDelegate = detailedProgressDelegate;
				this->textualProgressDelegate = textualProgressDelegate;
				this->textualProgressLambda = nullptr;
			}

			ProgressIndicator(ProgressDelegate progressDelegate, DetailedProgressDelegate detailedProgressDelegate = nullptr, TextualProgressLambda textualProgressLambda = nullptr)
			{
				this->progressDelegate = progressDelegate;
				this->detailedProgressDelegate = detailedProgressDelegate;
				this->textualProgressDelegate = nullptr;
				this->textualProgressLambda = textualProgressLambda;
			}

			void doProgress(double progress)
			{
				if (progressDelegate != nullptr) progressDelegate(progress);
			}
			void doDetailedProgress(int step, int loop, double reliability, double convergence)
			{
				if (detailedProgressDelegate != nullptr) detailedProgressDelegate(step, loop, reliability, convergence);
			}
			void doTextualProgress(ProgressType progressType, std::string text)
			{
				if (textualProgressDelegate != nullptr) textualProgressDelegate(progressType, text);
				if (textualProgressLambda != nullptr) textualProgressLambda(progressType, text);
			}
		};
	}
}

