#include "HohenbichlerNumIntCombiner.h"
#include "HohenbichlerNumInt.h"
#include "../Utils/probLibException.h"

namespace Deltares
{
    namespace Reliability
    {

        std::shared_ptr<DesignPoint> HohenbichlerNumIntCombiner::combineDesignPoints(combineAndOr combineMethodType, std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix, std::shared_ptr<ProgressIndicator> progress)
        {
            if (designPoints.empty()) throw probLibException("no design point in combiner");
            if (designPoints.size() == 1) return designPoints[0];

            const std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = getUniqueStochasts(designPoints);

            std::vector<std::shared_ptr<DesignPoint>> workDesignPoints;
            for (size_t i = 0; i < designPoints.size(); i++)
            {
                workDesignPoints.push_back(designPoints[i]);
            }

            auto designPoint = std::make_shared<DesignPoint>();
            auto hh = HohenbichlerNumInt();
            while (!workDesignPoints.empty())
            {
                long long i1max; long long i2max;
                findMaxCorrelatedDesignPoints(workDesignPoints, selfCorrelationMatrix, stochasts, i1max, i2max);
                designPoint = hh.AlphaHohenbichler(workDesignPoints[i1max], workDesignPoints[i2max], stochasts, selfCorrelationMatrix, combineMethodType);
                if (workDesignPoints.size() == 2)
                {
                    break;
                }
                workDesignPoints.erase(designPoints.begin() + i2max);
                workDesignPoints.erase(designPoints.begin() + i1max);
                workDesignPoints.push_back(designPoint);
            }

            for (size_t i = 0; i < designPoints.size(); i++)
            {
                designPoint->ContributingDesignPoints.push_back(designPoints[i]);
            }

            return designPoint;
        }

        void HohenbichlerNumIntCombiner::findMaxCorrelatedDesignPoints(std::vector<std::shared_ptr<DesignPoint>>& designPoints, std::shared_ptr<Statistics::SelfCorrelationMatrix> selfCorrelationMatrix,
            const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, long long& i1max, long long& i2max)
        {
            double rhoMax = -1.0;
            for (size_t i = 0; i < designPoints.size(); i++)
            {
                for (size_t j = i + 1; j < designPoints.size(); j++)
                {
                    const auto reorderedDesignPoint1 = designPoints[i]->getSampleForStochasts(stochasts);
                    const auto reorderedDesignPoint2 = designPoints[j]->getSampleForStochasts(stochasts);

                    double rho = 0.0;
                    const double betaDp1Dp2 = designPoints[i]->Beta * designPoints[j]->Beta;
                    for (size_t k = 0; k < stochasts.size(); k++)
                    {
                        auto corr = selfCorrelationMatrix->getSelfCorrelation(stochasts[k], designPoints[i], designPoints[j]);
                        rho += reorderedDesignPoint1->Values[k] * reorderedDesignPoint2->Values[k] * corr / betaDp1Dp2;
                    }
                    if (rho > rhoMax || (i == 0 && j == 1))
                    {
                        //
                        // For the first combination the parameters i1max, i2max and rhoMax are set
                        //
                        i1max = i;
                        i2max = j;
                        rhoMax = rho;
                    }
                }
            }
        }

    };
}

