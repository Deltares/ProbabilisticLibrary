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
            std::shared_ptr<GradientSettings> Settings = std::make_shared<GradientSettings>();
            std::vector<double> getGradient(std::shared_ptr<Models::ZModelRunner> modelRunner, std::shared_ptr<Sample> sample);
        };
    }
}



