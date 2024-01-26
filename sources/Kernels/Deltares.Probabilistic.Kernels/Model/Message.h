#pragma once
#include <string>

namespace Deltares
{
	namespace Models
	{
		enum MessageType { Debug, Info, Warning, Error };

		class Message
		{
		public:
			Message(MessageType type, std::string text)
			{
				this->Type = type;
				this->Text = text;
			}

			MessageType Type;
			std::string Text;
		};
	}
}

