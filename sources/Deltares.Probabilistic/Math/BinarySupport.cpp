#include "BinarySupport.h"

namespace Deltares::Mathematics
{
    unsigned int BinarySupport::RightmostZeroBit(unsigned int k)
    {
        unsigned int n = k | 0;

        unsigned int i = 1;
        while ((n & 1) == 1)
        {
            n >>= 1;
            ++i;
        }
        return i;
    }
}

