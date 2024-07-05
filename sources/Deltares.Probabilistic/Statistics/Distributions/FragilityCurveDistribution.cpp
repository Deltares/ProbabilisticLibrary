#include "FragilityCurveDistribution.h"

#include <algorithm>
#include <cmath>

#include "../StandardNormal.h"
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        void FragilityCurveDistribution::initializeForRun(std::shared_ptr<StochastProperties> stochast)
        {
            std::sort(stochast->FragilityValues.begin(), stochast->FragilityValues.end(), [](std::shared_ptr<FragilityValue> val1, std::shared_ptr<FragilityValue> val2) {return val1->X < val2->X; });


            Utils::SetDirtyLambda setDirtyFunction = [stochast]()
            {
                stochast->dirty = true;
            };

            for (std::shared_ptr<FragilityValue> fragilityValue : stochast->FragilityValues)
            {
                fragilityValue->setDirtyFunction(setDirtyFunction);
            }

            stochast->dirty = false;
        }

        bool FragilityCurveDistribution::isHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues)
        {
            for (size_t i = 1; i < fragilityValues.size(); i++)
            {
                if (!Numeric::NumericSupport::areEqual(fragilityValues[i]->Reliability, fragilityValues[i - 1]->Reliability, precision))
                {
                    return false;
                }
            }

            return true;
        }

        bool FragilityCurveDistribution::startsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues)
        {
            return fragilityValues.size() >= 2 && Numeric::NumericSupport::areEqual(fragilityValues[0]->Reliability, fragilityValues[1]->Reliability, precision);
        }

        bool FragilityCurveDistribution::isAscending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues)
        {
            return fragilityValues.size() >= 2 && Numeric::NumericSupport::isLess(fragilityValues[0]->Reliability, fragilityValues[fragilityValues.size() - 1]->Reliability, precision);
        }

        bool FragilityCurveDistribution::isDescending(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues)
        {
            return fragilityValues.size() >= 2 && Numeric::NumericSupport::isGreater(fragilityValues[0]->Reliability, fragilityValues[fragilityValues.size() - 1]->Reliability, precision);
        }

        bool FragilityCurveDistribution::endsHorizontal(std::vector<std::shared_ptr<FragilityValue>>& fragilityValues)
        {
            return fragilityValues.size() >= 2 && Numeric::NumericSupport::areEqual(fragilityValues[fragilityValues.size() - 1]->Reliability, fragilityValues[fragilityValues.size() - 2]->Reliability, precision);
        }

        bool FragilityCurveDistribution::isValid(std::shared_ptr<StochastProperties> stochast)
        {
            return !stochast->FragilityValues.size() > 1;
        }

        double FragilityCurveDistribution::getXFromU(std::shared_ptr<StochastProperties> stochast, double u)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->FragilityValues.empty())
            {
                return std::nan("");
            }

            if (stochast->FragilityValues.size() == 1)
            {
                return stochast->FragilityValues[0]->X;
            }

            if (this->isHorizontal(stochast->FragilityValues))
            {
                if (u > stochast->FragilityValues[0]->Reliability)
                {
                    return -std::numeric_limits<double>::infinity();
                }
                else
                {
                    return std::numeric_limits<double>::infinity();
                }
            }
            else if (startsHorizontal(stochast->FragilityValues) && u > stochast->FragilityValues[0]->Reliability && isDescending(stochast->FragilityValues))
            {
                return -std::numeric_limits<double>::infinity();
            }
            else if (startsHorizontal(stochast->FragilityValues) && u < stochast->FragilityValues[0]->Reliability && isAscending(stochast->FragilityValues))
            {
                return -std::numeric_limits<double>::infinity();
            }
            else if (endsHorizontal(stochast->FragilityValues) && u < stochast->FragilityValues.back()->Reliability && isDescending(stochast->FragilityValues))
            {
                return std::numeric_limits<double>::infinity();
            }
            else if (endsHorizontal(stochast->FragilityValues) && u > stochast->FragilityValues.back()->Reliability && isAscending(stochast->FragilityValues))
            {
                return std::numeric_limits<double>::infinity();
            }
            else
            {
                std::vector<double> xValues;
                std::vector<double> bValues;

                for (std::shared_ptr<FragilityValue> fragilityValue : stochast->FragilityValues)
                {
                    xValues.push_back(fragilityValue->X);
                    bValues.push_back(fragilityValue->Reliability);
                }

                return Numeric::NumericSupport::interpolate(u, bValues, xValues, true);
            }
        }

        double FragilityCurveDistribution::getUFromX(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->FragilityValues.empty())
            {
                return std::nan("");
            }

            std::vector<double> xValues;
            std::vector<double> bValues;

            for (std::shared_ptr<FragilityValue> fragilityValue : stochast->FragilityValues)
            {
                xValues.push_back(fragilityValue->X);
                bValues.push_back(fragilityValue->Reliability);
            }

            return Numeric::NumericSupport::interpolate(x, xValues, bValues, true);
        }

        double FragilityCurveDistribution::getCDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->FragilityValues.empty())
            {
                return std::nan("");
            }

            std::vector<double> xValues;
            std::vector<double> bValues;

            for (std::shared_ptr<FragilityValue> fragilityValue : stochast->FragilityValues)
            {
                xValues.push_back(fragilityValue->X);
                bValues.push_back(fragilityValue->Reliability);
            }

            double interpolatedReliability = Numeric::NumericSupport::interpolate(x, xValues, bValues, true);

            return StandardNormal::getPFromU(interpolatedReliability);
        }

        double FragilityCurveDistribution::getPDF(std::shared_ptr<StochastProperties> stochast, double x)
        {
            if (stochast->dirty)
            {
                initializeForRun(stochast);
            }

            if (stochast->FragilityValues.empty())
            {
                return std::nan("");
            }

            double delta = std::numeric_limits<double>::max();
            if (stochast->FragilityValues.size() == 1)
            {
                delta = std::max(1.0, std::abs(stochast->FragilityValues[0]->X) / 10);
            }
            else
            {
                for (size_t i = 0; i < stochast->FragilityValues.size() - 1; i++)
                {
                    delta = std::min(delta, stochast->FragilityValues[i + 1]->X - stochast->FragilityValues[i]->X);
                }

                delta /= 10;
            }

            double cdfLow = this->getCDF(stochast, x - delta / 2);
            double cdfHigh = this->getCDF(stochast, x + delta / 2);

            return (cdfHigh - cdfLow) / delta;
        }

        std::vector<double> FragilityCurveDistribution::getSpecialPoints(std::shared_ptr<StochastProperties> stochast)
        {
            std::vector<double> specialPoints;

            for (std::shared_ptr<FragilityValue> fragilityValue : stochast->FragilityValues)
            {
                specialPoints.push_back(fragilityValue->X);
            }

            return specialPoints;
        }
    }
}
