using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Deltares.Probabilistic.Logging
{
    public class Message : IDisposable
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

        public void Dispose()
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
