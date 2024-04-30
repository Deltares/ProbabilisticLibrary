#pragma once
#include "../../Deltares.Probabilistic/Model/ProxySettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public ref class ProxySettings
			{

			private:
				Utils::Wrappers::SharedPointerProvider<Models::ProxySettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Models::ProxySettings());

			public:
				ProxySettings() { }
				~ProxySettings() { this->!ProxySettings(); }
				!ProxySettings() { delete shared; }

				property bool IsProxyModel
				{
					bool get() { return shared->object->IsProxyModel; }
					void set(bool value) { shared->object->IsProxyModel = value; }
				}

				property bool ShouldUpdateFinalSteps
				{
					bool get() { return shared->object->ShouldUpdateFinalSteps; }
					void set(bool value) { shared->object->ShouldUpdateFinalSteps = value; }
				}

				property double ThresholdOffset
				{
					double get() { return shared->object->ThresholdOffset; }
					void set(double value) { shared->object->ThresholdOffset = value; }
				}

				std::shared_ptr<Models::ProxySettings> GetSettings()
				{
					return shared->object;
				}
			};
		}
	}
}
