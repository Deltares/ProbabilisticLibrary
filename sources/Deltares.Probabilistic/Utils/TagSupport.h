#pragma once
#include <functional>
#include <cstdint>

namespace Deltares
{
    namespace Utils
    {
        typedef std::function<void(intptr_t)> ReleaseTagLambda;

        class TagSupport
        {
        public:
            static void setReleaseTagLambda(ReleaseTagLambda tagLambda)
            {
                TagSupport::tagLambda = tagLambda;
            }

            static void releaseTag(intptr_t tag)
            {
                if (tagLambda != nullptr)
                {
                    tagLambda(tag);
                }
            }

        private:
            inline static ReleaseTagLambda tagLambda = nullptr;
        };
    }
}

