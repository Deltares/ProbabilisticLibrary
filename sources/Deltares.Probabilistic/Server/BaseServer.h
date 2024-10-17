#pragma once
#include <memory>
#include <string>

namespace Deltares
{
    namespace Server
    {
        class BaseServer
        {
        public:
            virtual int GetNewObjectId(int handlerIndex) { return 0; }
            //virtual std::shared_ptr<BaseHandler> GetHandler(std::string objectType) { return nullptr; }
        };
    }
}

