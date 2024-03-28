#include <string>
#include <math.h>
#include <float.h>
#include <vector>
#include "combineElements.h"
#include "Hohenbichler.h"
#include "../Utils/probLibException.h"
#include "../Statistics/StandardNormal.h"
#include "../Math/basic_math.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        //> Method for combining two elements with partial correlation
        cmbResult combineElements::combineTwoElementsPartialCorrelation(alphaBeta& element1,
            alphaBeta& element2, const vector1D& rhoP, const combineAndOr combAndOr)
        {
            vector1D alphaI; vector1D alphaII;
            return combineTwoElementsPartialCorrelation(element1, element2, rhoP, combAndOr, alphaI, alphaII);
        }

        //> Method for combining two elements with partial correlation
        cmbResult combineElements::combineTwoElementsPartialCorrelation(alphaBeta& element1,
            alphaBeta& element2, const vector1D& rhoP, const combineAndOr combAndOr,
            vector1D& alphaI, vector1D& alphaII)
            //
            //   INPUT/OUTPUT VARIABLES
            //
            // element1  : Reliability index and alpha of element 1
            // element2  : Reliability index and alpha of element 2
            // rhoP(:)   : Autocorrelation of the stochastic variables between element 1 and element 2
            // combAndOr : Combination type, And or Or
            // alphaI(:) : AlphaI values of the combined elements in case of a spatial correlation
            // alphaII(:): AlphaII values of the combined elements in case of a spatial correlation

            //
            //   LOCAL VARIABLES
            //
        {
            const double epsilon = 0.01;
            int failureHohenbichler = 0;
            //
            // Determine vector size
            //
            size_t nStochasts = element1.size();
            checkArraySizes(nStochasts, element2.size(), rhoP.size());
            //
            // Allocate temp vectors
            //
            alphaBeta element3; // returned value
            element3.setAlpha(vector1D(nStochasts));
            vector1D alphaX1 = vector1D(nStochasts);
            vector1D alphaX2 = vector1D(nStochasts);
            //
            //   Determine the correlation of the system
            //
            double rho = element1.sumOfInners(element2, rhoP);
            //
            //   Part 1:
            //   Determining the reliability index
            //
            double pf1 = element1.getQ();
            double pf2 = element2.getQ();
            //
            //   The largest value of beta1 or beta2 is used for computing the probability of failure
            //
            auto pb = setLargestBeta(element1.getBeta(), element2.getBeta(), pf1, pf2);
            //
            //   Computation of P( Z_2 < 0 | Z_1 < 0)
            //
            //auto p = progress();
            auto hh = Hohenbichler();
            auto pf2pf1 = hh.PerformHohenbichler(pb.second, pb.first, rho);
            if (pf2pf1.second != 0) failureHohenbichler++;
            //
            //   Computation of combined failure probability (AND/OR)
            //
            double pfCombined = combinedFailure(combAndOr, pf1, pf2, pb.first, pf2pf1.first);

            //
            //   Compute reliability index
            //
            element3.setBeta(StandardNormal::getUFromQ(pfCombined));

            //
            //   Part 2:
            //   Determining alpha-values for the correlated and uncorrelated parts of the combination
            //
            for (size_t k = 0; k < nStochasts; k++)
            {
                //
                // Check if stochast is of influence
                if (element1.getAlphaI(k) == 0.00 && element2.getAlphaI(k) == 0.0)
                {
                    alphaX1(k) = 0.0;
                    alphaX2(k) = 0.0;
                }
                else
                {
                    //
                    //          Correlated part
                    double beta1Delta = element1.getBeta() - element1.getAlphaI(k) * epsilon;
                    pf1 = StandardNormal::getQFromU(beta1Delta);
                    //
                    double beta2Delta = element2.getBeta() - element2.getAlphaI(k) * epsilon * rhoP(k);
                    pf2 = StandardNormal::getQFromU(beta2Delta);
                    //
                    pb = setLargestBeta(beta1Delta, beta2Delta, pf1, pf2);
                    //
                    //           Computation of P( Z_2 < - alpha1(k) * epsilon | Z_1 < -alpha2(k) * epsilon * rhoP(k) )
                    //
                    pf2pf1 = hh.PerformHohenbichler(pb.second, pb.first, rho);
                    if (pf2pf1.second != 0) failureHohenbichler++;
                    //
        //           Computation of combined failure probability (AND/OR)
        //
                    double pfxk = combinedFailure(combAndOr, pf1, pf2, pb.first, pf2pf1.first);
                    //
                    double betaxk = StandardNormal::getUFromQ(pfxk);
                    alphaX1(k) = (element3.getBeta() - betaxk) / epsilon;
                    //
                    //           Uncorrelated part
                    if (fabs(rhoP(k)) < 1.0)
                    {
                        //
                        beta1Delta = element1.getBeta();
                        pf1 = StandardNormal::getQFromU(beta1Delta);
                        //
                        beta2Delta = element2.getBeta() - element2.getAlphaI(k) * epsilon * sqrt(1.0 - pow(rhoP(k), 2));
                        pf2 = StandardNormal::getQFromU(beta2Delta);
                        //
                        pb = setLargestBeta(beta1Delta, beta2Delta, pf1, pf2);
                        //
                        //               Computation of P( Z_2 < 0 | Z_1 < -alpha2(k) * epsilon * sqrt( 1 - rhoP(k))^2) )
                        pf2pf1 = hh.PerformHohenbichler(pb.second, pb.first, rho);
                        if (pf2pf1.second != 0) failureHohenbichler++;
                        //
                        //               Computation of combined failure probability (AND/OR)
                        //
                        pfxk = combinedFailure(combAndOr, pf1, pf2, pb.first, pf2pf1.first);

                        betaxk = StandardNormal::getUFromQ(pfxk);
                        alphaX2(k) = (element3.getBeta() - betaxk) / epsilon;
                    }
                    else
                    {
                        alphaX2(k) = 0.0;
                    }
                }
            }
            //
            //   Correct alpha for neglected area of fail domain
            //
            for (size_t k = 0; k < nStochasts; k++)
            {
                double alphaFactor;
                if (element1.getAlphaI(k) != 0.0 || element2.getAlphaI(k) != 0.0)
                {
                    if (fabs(element1.getAlphaI(k)) >= fabs(element2.getAlphaI(k)))
                    {
                        alphaFactor = 1.0 - rhoP(k) * fabs((element2.getAlphaI(k) / element1.getAlphaI(k)));
                    }
                    else
                    {
                        alphaFactor = 1.0 - rhoP(k) * fabs((element1.getAlphaI(k) / element2.getAlphaI(k)));
                    }

                    double alphaMultiplier = 1.0 + std::max(0.0, 0.1710 + 0.03160 * std::min(element1.getBeta(), element2.getBeta()))
                        * exp(-pow((element1.getBeta() - element2.getBeta()) / 0.40, 2)) * alphaFactor;

                    alphaX1(k) *= alphaMultiplier;
                    alphaX2(k) *= alphaMultiplier;
                }
            }
            //
            //   Determine alpha
            //   alphaC(k)= (+ or -) sqrt(alpha1(k)^2 + alpha2(k)^2)
            for (size_t k = 0; k < nStochasts; k++)
            {
                element3.setAlpha(k, hypot(alphaX1(k), alphaX2(k)));
                if (element3.getAlphaI(k) > 0.0)
                {
                    // Get the sign of the dominant part
                    double alphaSign = fabs(alphaX1(k)) > fabs(alphaX2(k)) ? alphaX1(k) : alphaX2(k);

                    element3.setAlpha(k, sign(element3.getAlphaI(k), alphaSign));
                }
            }
            //
            //   Normalise vector with alpha values
            //
            element3.normalize();

            if (alphaI.size() == nStochasts && alphaII.size() == nStochasts)
            {
                for (size_t k = 0; k < nStochasts; k++)
                {
                    double factor = element3.getAlphaI(k) / hypot(alphaX1(k), alphaX2(k));
                    alphaI(k) = alphaX1(k) * factor;
                    alphaII(k) = alphaX2(k) * factor;
                }
            }
            return { element3, failureHohenbichler };
        }

        // Auxiliary routine: the arrays should all have the same size
        void combineElements::checkArraySizes(const size_t nStochasts, const size_t sizeAlpha2, const size_t sizeRhoP)
        {
            if (nStochasts != sizeAlpha2)
            {
                throw probLibException("Array size alpha2 differs from alpha1");
            }
            if (nStochasts != sizeRhoP)
            {
                throw probLibException("Array size rhoP differs from alpha1");
            }
        }

        // Auxiliary routine: set the values according to the largest beta
        std::pair<double, double> combineElements::setLargestBeta(const double beta1, const double beta2, const double pf1, const double pf2)
        {
            if (beta1 > beta2)
            {
                return std::pair<double, double>(pf1, beta2);
            }
            else
            {
                return std::pair<double, double>(pf2, beta1);
            }
        }

        // Auxiliary function: determine the combined probability of failure
        double combineElements::combinedFailure(const combineAndOr combAndOr, const double pf1,
            const double pf2, const double pfu, const double pf2pf1)
        {
            switch (combAndOr)
            {
            case combineAndOr::combOr:
                return pf1 + pf2 - pfu * pf2pf1;

            default:
                return pfu * pf2pf1;
            }
        }

        //>
        //! This method combines multiple elements with partial correlation
        cmbResult combineElements::combineMultipleElements(const elements& Elements,
            const vector1D& rho, const combineAndOr combAndOr)
        {
            // Elements(:)       : Reliability index and alpha per element
            // rho(:)            : Correlation data
            // combAndOrIn       : Combination type, And or Or
            //
            // Get the vector dimensions
            //
            size_t nrElements = Elements.size();
            size_t nrStochasts = Elements[0].size();

            if (nrElements == 1)
            {
                return { Elements[0], 0 };
            }
            else
            {
                //
                // Set local data
                int failures = 0;
                size_t nrElementsComp = nrElements;
                auto local = Elements;
                cmbResult ab;
                ab.ab.setAlpha(vector1D(nrStochasts));

                //
                // Continuously combine the two elements with the largest correlation
                //
                size_t i1 = SIZE_MAX;     // Keep static analyser happy
                size_t i2 = SIZE_MAX;
                for (size_t iElement = nrElements - 1; iElement > 0; iElement--)
                {
                    //
                    // Calculate the combination of two elements which has
                    // together the largest correlation
                    //
                    calculateCombinationWithLargestCorrelation(rho, nrElementsComp, local, i1, i2);
                    //
                    // Combine these two elements with partial correlation
                    //
                    auto a1 = vector1D(nrStochasts); auto a2 = vector1D(nrStochasts);
                    ab = combineTwoElementsPartialCorrelation(local[i1], local[i2], rho, combAndOr, a1, a2);
                    failures += ab.n;

                    //
                    // Compose the new vector betaLocal and alphaLocal
                    // The elements i1 and i2 are removed from the vector betaLocal and the combined result is added last
                    // betaLocal(1:i1-1) stays unchanged
                    //
                    for (size_t i = i1; i < i2 - 1; i++)
                    {
                        local[i] = local[i + 1];
                    }
                    for (size_t i = i2 - 1; i < nrElementsComp - 2; i++)
                    {
                        local[i] = local[i + 2];
                    }

                    local[nrElementsComp - 2] = ab.ab;

                    nrElementsComp--;
                }
                return { ab.ab, failures };
            }
        }

        //>
        //! This method combines multiple elements with spatial correlation
        cmbResult combineElements::combineMultipleElementsSpatialCorrelated(const elements& Element,
            const std::vector<std::vector<vector1D>>& rho, const combineAndOr combAndOr)
        {
            // betaElement(:)    : Reliability index per element
            // alphaElement(:,:) : Alpha vector per element
            // rho(:,:,:)        : Correlation data
            // beta              : Reliability index after combining over elements
            // alpha(:)          : Alpha vector after combining over elements
            // combAndOrIn       : Combination type, And or Or
            //
            // Get the vector dimensions
            //
            size_t nrElements = Element.size();
            size_t nrStochasts = Element[0].size();

            if (nrElements == 1)
            {
                return { Element[0], 0 };
            }
            else
            {
                //
                // Set local data
                size_t nrElementsComp = nrElements;
                auto  Local = Element;
                auto rhoLocal = rho;
                auto rhoTMP = rho;
                auto alphaI = vector1D(nrStochasts);
                auto alphaII = vector1D(nrStochasts);
                cmbResult ab; // returned value
                int failures = 0;

                //
                // Continuously combine the two elements with the largest correlation
                //
                size_t i1 = SIZE_MAX;     // Keep static analyser happy
                size_t i2 = SIZE_MAX;
                for (size_t iElement = nrElements - 1; iElement > 0; iElement--)
                {
                    //
                    // Calculate the combination of two elements which has
                    // together the largest correlation
                    //
                    calculateCombinationWithLargestCorrelationSpatialCorrelated(rhoLocal, nrElementsComp, Local, i1, i2);
                    //
                    // Combine these two elements with partial correlation
                    //
                    ab = combineTwoElementsPartialCorrelation(Local[i1], Local[i2], rhoLocal[i1][i2], combAndOr, alphaI, alphaII);
                    failures += ab.n;
                    //
                    // Compose the new vector betaLocal and alphaLocal
                    // The wind directions i1 and i2 are removed from the vector betaLocal and the combined result is added last
                    // betaLocal(1:i1-1) stays unchanged
                    //
                    for (size_t i = i1; i < i2 - 1; i++)
                    {
                        Local[i] = Local[i + 1];
                    }
                    for (size_t i = i2 - 1; i < nrElementsComp - 2; i++)
                    {
                        Local[i] = Local[i + 2];
                    }

                    Local[nrElementsComp - 2] = ab.ab;

                    //
                    // Compose the new correlation matrix
                    rhoTMP = rhoLocal;
                    for (size_t j = 0; j < nrElementsComp; j++)
                    {
                        if (j != i1 && j != i2)
                        {
                            for (size_t i = 0; i < nrStochasts; i++)
                            {
                                double a = (rhoLocal[i2][j](i) - rhoLocal[i1][i2](i) * rhoLocal[i1][j](i)) / sqrt(1.0 - rhoLocal[i1][i2](i) * rhoLocal[i1][i2](i));
                                if (fabs(ab.ab.getAlphaI(i)) > 0.000001)
                                {
                                    rhoTMP[j][i1](i) = (rhoLocal[i1][j](i) * alphaI(i) + a * alphaII(i)) / ab.ab.getAlphaI(i);
                                }
                                else
                                {
                                    rhoTMP[j][i1](i) = rhoLocal[i1][j](i);
                                }
                            }
                        }
                        else
                        {
                            for (size_t i = 0; i < nrStochasts; i++)
                            {
                                rhoTMP[j][i1](i) = 1.0;
                            }
                        }
                    }
                    if (i2 < i1)
                    {
                        size_t i = i1; // TODO use swap
                        i1 = i2;
                        i2 = i;
                    }
                    rhoLocal = rhoTMP;
                    for (size_t i = i1; i < i2 - 1; i++)
                    {
                        rhoLocal[i] = rhoTMP[i + 1];
                    }
                    for (size_t i = i2 - 1; i < nrElementsComp - 2; i++)
                    {
                        rhoLocal[i] = rhoTMP[i + 2];
                    }

                    for (size_t j = 0; j < nrElementsComp; j++)
                    {
                        rhoLocal[nrElementsComp - 2][j] = rhoTMP[j][i1];
                    }

                    rhoTMP = rhoLocal;       // Make a copy to overcome stack errors
                    for (size_t k = 0; k < nrElementsComp; k++)
                    {
                        for (size_t i = i1; i < i2 - 1; i++)
                        {
                            rhoLocal[k][i] = rhoTMP[k][i + 1];
                        }
                        for (size_t i = i2 - 1; i < nrElementsComp - 2; i++)
                        {
                            rhoLocal[k][i] = rhoTMP[k][i + 2];
                        }
                    }

                    for (size_t j = 0; j < nrElementsComp; j++)
                    {
                        rhoLocal[j][nrElementsComp - 2] = rhoLocal[nrElementsComp - 2][j];
                    }
                    rhoLocal[nrElementsComp - 2][nrElementsComp - 2] = vector1D(nrStochasts);
                    for (size_t i = 0; i < nrStochasts; i++)
                    {
                        rhoLocal[nrElementsComp - 2][nrElementsComp - 2](i) = 1.0;
                    }

                    nrElementsComp--;
                }
                return { ab.ab, failures };
            }
        }

        //>
        //! This method computes the correlation between elements and returns the index of the two elements with the highest correlation.
        //! The input is a matrix of alpha values, one vector per element, and the autocorrelation of the random variables between elements.
        //! The correlation between two elements, element 1 and 2, is computed as follows:
        //! \f$ \rho _{1,2}  = \sum\limits_{j = 1}^n {\rho _{1,2,j}  \cdot \alpha _{1,j} }  \cdot \alpha _{2,j} \f$
        //! where j sums over the n random variables, \f$\rho _{1,2,j} \f$ refers to the correlation of variable j in element 1 with variable j
        //! element 2, \f$ \alpha _{1,j} \f$ refers to the influence coefficient of variable j in element 1, and \f$ \alpha _{2,j} \f$ refers to
        //! the influence coefficient of variable j in element 2.
        void combineElements::calculateCombinationWithLargestCorrelation(const vector1D& rhoP,
            const size_t nElements, const std::vector<alphaBeta>& ab, size_t& i1max, size_t& i2max)
        {
            // rhoP(nStochasts)  : Autocorrelation the random variables between elements
            // nElements         : Number of elements to be combined (for instance tidal periods)
            // alpha(:,:)        : Alpha vector per element
            // i1max             : Index of first element with the largest correlation
            // i2max             : Index of second element with the largest correlation
            //
            // Two elements can't be computed if there is only one element
            //
            if (nElements < 2)
            {
                throw probLibException("The method calculateCombinationWithLargestCorrelation is called with only one element");
            }
            //
            // Initialize rhoMax and the indices for the maximum element
            //
            double rhoMax = -1.0;
            i1max = SIZE_MAX;
            i2max = SIZE_MAX;
            //
            //   Determine which two elements have the highest correlation
            //
            for (size_t i1 = 0; i1 < nElements - 1; i1++)
            {
                for (size_t i2 = i1 + 1; i2 < nElements; i2++)
                {
                    //
                    // Compute correlation
                    //
                    double rhoT = ab[i1].sumOfInners(ab[i2], rhoP);

                    if (rhoT > rhoMax || (i1 == 0 && i2 == 1))
                    {
                        //
                        // For the first combination the parameters i1max, i2max and rhoMax are set
                        //
                        i1max = i1;
                        i2max = i2;
                        rhoMax = rhoT;
                    }
                }
            }
        }

        //>
        //! This method computes the correlation between elements and returns the index of the two elements with the highest correlation.
        void combineElements::calculateCombinationWithLargestCorrelationSpatialCorrelated(
            const std::vector<std::vector<vector1D>>& rhoP, const size_t nElements, const elements& elements,
            size_t& i1max, size_t& i2max)
        {
            // rhoP(:, :, :)  : Autocorrelation the random variables between elements
            // nElements      : Number of elements to be combined (for instance tidal periods)
            // alpha(:,:)     : Alpha vector per element
            // i1max          : Index of first element with the largest correlation
            // i2max          : Index of second element with the largest correlation
            //
            // Two elements can't be computed if there is only one element
            //
            if (nElements < 2)
            {
                throw probLibException("The method calculateCombinationWithLargestCorrelationSpatialCorrelated is called with only one element");
            }
            //
            // Initialize rhoMax and the indices for the maximum element
            //
            double rhoMax = -1.0;
            i1max = SIZE_MAX;
            i2max = SIZE_MAX;
            //
            //   Determine which two elements have the highest correlation
            //
            for (size_t i1 = 0; i1 < nElements - 1; i1++)
            {
                for (size_t i2 = i1 + 1; i2 < nElements; i2++)
                {
                    //
                    // Compute correlation
                    //
                    double rhoT = elements[i1].sumOfInners(elements[i2], rhoP[i1][i2]);
                    if (rhoT > rhoMax || (i1 == 0 && i2 == 1))
                    {
                        //
                        // For the first combination the parameters i1max, i2max and rhoMax are set
                        //
                        i1max = i1;
                        i2max = i2;
                        rhoMax = rhoT;
                    }
                }
            }
        }

        //>
        //! This method calculates the reliability index (beta) and alpha values combining over elements
        cmbResult combineElements::combineMultipleElementsFull(const elements& Elements,
            const combineAndOr combAndOr)
        {
            // Elements(:)       : Reliability index and alpha per element
            // combAndOrIn       : Combination type, And or Or

            //
            // Get the vector dimensions
            //
            size_t nrStochasts = Elements[0].size();
            //
            // Allocate vectors and matrices
            // Get the correlation coefficients of all the strength variables
            //
            auto rhoP = vector1D(nrStochasts);
            rhoP.assign(1.0);

            return combineMultipleElements(Elements, rhoP, combAndOr);
        }

        //>
        //! This method calculates the reliability index (beta) and alpha values combining over elements
        cmbResult combineElements::combineMultipleElementsProb(elements& Elements,
            const std::vector<double>& percentages, const combineAndOr combAndOr)
        {
            // Elements(:)        : Reliability index and alpha per element
            // percentages(:)     : Array of percentages
            // combAndOrIn        : Combination type, And or Or

            //
            // Get the vector dimensions
            //
            size_t nrStochasts = Elements[0].size();
            size_t nrElements = Elements.size();
            //
            // Allocate vectors and matrices
            //
            auto rhoP = vector1D(nrStochasts);

            //
            // Adjust the beta values of the input with the probability
            //
            auto adjustedElement = std::vector<alphaBeta>(nrElements);
            for (size_t i = 0; i < nrElements; i++)
            {
                double adjustedProbability = Elements[i].getQ();
                adjustedProbability *= percentages[i] / 100.0;
                adjustedElement[i] = alphaBeta(StandardNormal::getUFromQ(adjustedProbability), Elements[i].getAlpha());
            }

            return combineMultipleElements(adjustedElement, rhoP, combAndOr);
        }

    }
}
