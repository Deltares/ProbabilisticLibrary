#pragma once
#include "ProxySettings.h"
#include "../../Deltares.Probabilistic/Model/RunSettings.h"
#include "../../Deltares.Probabilistic/Model/Message.h"
#include "../Utils/SharedPointerProvider.h"
#include "Message.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class RunSettings
			{
			private:
				SharedPointerProvider<Models::RunSettings>* shared = new SharedPointerProvider(new Models::RunSettings());

			public:
				RunSettings() { }
				~RunSettings() { this->!RunSettings(); }
				!RunSettings() { delete shared; }

				property int MaxChunkSize
				{
					int get() { return shared->object->MaxChunkSize; }
					void set(int value) { shared->object->MaxChunkSize = value; }
				}

				property int MaxParallelProcesses
				{
					int get() { return shared->object->MaxParallelProcesses; }
					void set(int value) { shared->object->MaxParallelProcesses = value; }
				}

				property bool SaveConvergence
				{
					bool get() { return shared->object->SaveConvergence; }
					void set(bool value) { shared->object->SaveConvergence = value; }
				}

				property bool SaveEvaluations
				{
					bool get() { return shared->object->SaveEvaluations; }
					void set(bool value) { shared->object->SaveEvaluations = value; }
				}

				property bool SaveMessages
				{
					bool get() { return shared->object->SaveMessages; }
					void set(bool value) { shared->object->SaveMessages = value; }
				}

				property int MaxMessages
				{
					int get() { return shared->object->MaxMessages; }
					void set(int value) { shared->object->MaxMessages = value; }
				}

				property Wrappers::MessageType LowestMessageType
				{
					Wrappers::MessageType get()
					{
						switch (shared->object->LowestMessageType)
						{
						case Models::Debug: return Wrappers::MessageType::Debug;
						case Models::Info: return Wrappers::MessageType::Info;
						case Models::Warning: return Wrappers::MessageType::Warning;
						case Models::Error: return Wrappers::MessageType::Error;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
					void set(Wrappers::MessageType value)
					{
						switch (value)
						{
						case Wrappers::MessageType::Debug: shared->object->LowestMessageType = Models::MessageType::Debug; break;
						case Wrappers::MessageType::Info: shared->object->LowestMessageType = Models::MessageType::Info; break;
						case Wrappers::MessageType::Warning: shared->object->LowestMessageType = Models::MessageType::Warning; break;
						case Wrappers::MessageType::Error: shared->object->LowestMessageType = Models::MessageType::Error; break;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
				}

				Wrappers::ProxySettings^ ProxySettings = gcnew Wrappers::ProxySettings();

				bool IsValid()
				{
					return shared->object->isValid();
				}

				std::shared_ptr<Models::RunSettings> GetSettings()
				{
					shared->object->proxySettings = this->ProxySettings->GetSettings();
					return shared->object;
				}
			};

			public interface class IHasRunSettings
			{
				property Deltares::Models::Wrappers::RunSettings^ RunSettings;
			};
		}
	}
}


