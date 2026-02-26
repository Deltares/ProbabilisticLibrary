// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
using System;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Logging
{
    public class Message
    {
        private int id = 0;

        public Message()
        {
            this.id = Interface.Create("message");
        }

        internal Message(int id)
        {
            this.id = id;
        }

        ~Message()
        {
            Interface.Destroy(id);
        }

        internal int GetId()
        {
            return id;
        }

        public MessageType Type
        {
            get { return MessageTypeConverter.ConvertFromString(Interface.GetStringValue(id, "type")); }
            set { Interface.SetStringValue(id, "type", MessageTypeConverter.ConvertToString(value)); }
        }

        public string Text
        {
            get { return Interface.GetStringValue(id, "text"); }
            set { Interface.SetStringValue(id, "text", value); }
        }

        public string Subject
        {
            get { return Interface.GetStringValue(id, "subject"); }
            set { Interface.SetStringValue(id, "subject", value); }
        }
    }
}
