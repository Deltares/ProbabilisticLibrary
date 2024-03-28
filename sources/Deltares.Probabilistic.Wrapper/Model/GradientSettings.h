#pragma once

#include <memory>

#include "../../Deltares.Probabilistic/Model/GradientSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public enum class GradientType
			{
				OneDirection,
				TwoDirections
			};

			public ref class GradientSettings
			{
			private:
				Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>* shared = new Utils::Wrappers::SharedPointerProvider<Models::GradientSettings>(new Models::GradientSettings());
			public:
				GradientSettings() {}
				~GradientSettings() { this->!GradientSettings(); }
				!GradientSettings() { delete shared; }

				property double StepSize
				{
					double get() { return shared->object->StepSize; }
					void set(double value) { shared->object->StepSize = value; }
				}

				property Wrappers::GradientType GradientType
				{
					Wrappers::GradientType get()
					{
						switch (shared->object->gradientType)
						{
						case Deltares::Models::GradientType::OneDirection: return Wrappers::GradientType::OneDirection;
						case Deltares::Models::GradientType::TwoDirections: return Wrappers::GradientType::TwoDirections;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
					void set(Wrappers::GradientType value)
					{
						switch (value)
						{
						case Wrappers::GradientType::OneDirection: shared->object->gradientType = Models::GradientType::OneDirection; break;
						case Wrappers::GradientType::TwoDirections: shared->object->gradientType = Models::GradientType::TwoDirections; break;
						default: throw gcnew System::NotSupportedException("gradient type");
						}
					}
				}

				std::shared_ptr<Models::GradientSettings> GetSettings()
				{
					return shared->object;
				}
			};
		}
	}
}



