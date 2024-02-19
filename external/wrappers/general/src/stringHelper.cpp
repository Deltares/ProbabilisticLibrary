#include "stringHelper.h"
#include "../../../src/utils/basic_math.h"

using namespace Deltares::ProbLibCore;

void fillErrorMessage(tError & error, const std::string s)
{
    size_t length = min(s.length(), ERRORMSGLENGTH);
    for (size_t i = 0; i < length; i++)
    {
        error.errorMessage[i] = s[i];
    }
    size_t last = min(s.length(), ERRORMSGLENGTH-1);
    error.errorMessage[last] = char(0);
}
