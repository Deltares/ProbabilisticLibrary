#include <vector>

#include "GradientSettings.h"
#include "ZModelRunner.h"


namespace Deltares
{
    namespace Models
    {
        class GradientCalculator
        {
        public:
            GradientCalculator() {}
            GradientSettings* Settings = nullptr;
            std::vector<double> getGradient(Models::ZModelRunner* modelRunner, Sample* sample);
        };
    }
}



