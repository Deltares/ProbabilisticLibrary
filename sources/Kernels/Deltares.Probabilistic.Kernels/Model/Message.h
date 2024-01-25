#pragma once
#include <string>

namespace Deltares
{
	namespace Models
	{
		enum MessageType { Error, Warning, Info, Debug };

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

