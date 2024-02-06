#pragma once
#include "../../Deltares.Probabilistic/Model/RunSettings.h"
#include "../../Deltares.Probabilistic/Model/Message.h"
#include "../Utils/SharedPointerProvider.h"
#include "MessageWrapper.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class RunSettingsWrapper
			{
			private:
				RunSettings* m_settings;
				SharedPointerProvider<RunSettings>* sharedPointer = new SharedPointerProvider<RunSettings>();

			public:
				RunSettingsWrapper() { m_settings = new RunSettings(); }
				~RunSettingsWrapper() { this->!RunSettingsWrapper(); }
				!RunSettingsWrapper() { delete sharedPointer; }

				property int MaxChunkSize
				{
					int get() { return m_settings->MaxChunkSize; }
					void set(int value) { m_settings->MaxChunkSize = value; }
				}

				property int MaxParallelProcesses
				{
					int get() { return m_settings->MaxParallelProcesses; }
					void set(int value) { m_settings->MaxParallelProcesses = value; }
				}

				property bool SaveConvergence
				{
					bool get() { return m_settings->SaveConvergence; }
					void set(bool value) { m_settings->SaveConvergence = value; }
				}

				property bool SaveEvaluations
				{
					bool get() { return m_settings->SaveEvaluations; }
					void set(bool value) { m_settings->SaveEvaluations = value; }
				}

				property bool SaveMessages
				{
					bool get() { return m_settings->SaveMessages; }
					void set(bool value) { m_settings->SaveMessages = value; }
				}

				property int MaxMessages
				{
					int get() { return m_settings->MaxMessages; }
					void set(int value) { m_settings->MaxMessages = value; }
				}

				property Wrappers::MessageType LowestMessageType
				{
					Wrappers::MessageType get()
					{
						switch (m_settings->LowestMessageType)
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
						case Wrappers::MessageType::Debug: m_settings->LowestMessageType = Models::MessageType::Debug; break;
						case Wrappers::MessageType::Info: m_settings->LowestMessageType = Models::MessageType::Info; break;
						case Wrappers::MessageType::Warning: m_settings->LowestMessageType = Models::MessageType::Warning; break;
						case Wrappers::MessageType::Error: m_settings->LowestMessageType = Models::MessageType::Error; break;
						default: throw gcnew System::NotSupportedException("Message type");
						}
					}
				}

				std::shared_ptr<RunSettings> GetSettings()
				{
					return sharedPointer->getSharedPointer(m_settings);
				}
			};
		}
	}
}


