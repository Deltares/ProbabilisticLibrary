#include <cmath>
#include "gumble.h"
#include "../probFuncs/conversions.h"
#include "../utils/basic_math.h"
#include "../utils/probLibException.h"

namespace Deltares {
    namespace ProbLibCore {

        // Inverse gumbel distribution
        // Computes the inverse CDF of the Gumbel (extreme-value type I) distribution
        // Gumbel distribution:
        //
        // CDF: \f$ F(x) = a e^{-a(x-b)-e^{-a(x-b)}} \f$
        //
        // Inverse:  \f$ x = b - {\ln[-\ln(p)] / a} \f$, where p is non-exceedance probability
        double gumbel::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);
            if (u > uLimit)
            {
                q = max(q, qMin);
                return a - log(q) / b;
            }
            else
            {
                p = max(p, qMin);
                return a - log(-log(p)) / b;
            }
        }

        // Inverse gumbel distribution
        // Computes the inverse CDF of the Gumbel (extreme-value type I) distribution
        // Gumbel distribution:
        //
        // PDF: \f$ F(x) = a e^{-a(x-b)-e^{-a(x-b)}} \f$
        //
        // Inverse:  \f$ x = b - {\ln[-ln(p)] / a} \f$, where p is non-exceedance probability
        //
        // <b>note about this code:</b>
        // the note described for the gumbelInverse function holds for this function as well.
        //
        // This function accepts as input the mean and standard deviation of the variable, and
        // computes the parameters of the gumbel distribution from these values, using information
        // about the first and second moment of the distribution
        double gumbelII::getValue(double u) const
        {
            // Convert from mean and standard of actual variable to parameters of the distribution
            double alfpar = M_PI / b / sqrt(6.0);
            double upar = a - EMconstant / alfpar;

            double p; double q;
            conversions::PQfromBeta(u, p, q);

            if (u > uLimit)
            {
                q = max(q, tiny);
                return upar - log(q) / alfpar;
            }
            else
            {
                p = max(p, tiny);
                return upar - log(-log(p)) / alfpar;
            }
        }

        // Inverse modified gumbel distribution
        //
        // Computes the inverse CDF of the modified Gumbel distribution
        //
        // Modified Gumbel distribution
        //
        // CDF: \f$ F(x;Kr)= exp[-exp[-Kr(x;a,b,c)]] \f$
        //
        // \f$ Kr(x;a,b,c)=ax^2+bx+c \f$ 
        //
        // Inverse:  \f$ x = [-b+\sqrt(b^2-4ac')]/2a \f$, for \f$ (b^2-4ac')>0 \f$
        //
        // \f$ x = -b/2a \f$, for \f$ (b^2-4ac')<0 \f$
        double gumbelModified::getValue(const double u) const
        {
            double p; double q;
            conversions::PQfromBeta(u, p, q);
            double cprime;
            if (u > uLimit)
            {
                double qd = max(q, qMin);
                cprime = c + log(qd);
            }
            else
            {
                double pd = max(p, qMin);
                cprime = c + log(-log(pd));
            }

            double crit = b * b - (4.0 * a * cprime);

            if (crit > 0)
            {
                return (-b + sqrt(crit)) / (2.0 * a);
            }
            else
            {
                return -b / (2.00 * a);
            }
        }

        // Truncated modified Gumbel
        gumbelTruncatedModified::gumbelTruncatedModified(const double p1, const double p2, const double p3, const double p4) :
            gm(gumbelModified(p1, p2, p3)), d(p4)
        {
            if (d <= 0) { throw probLibException("parameter 4 in gumbelTruncatedModified must be > 0"); }
        }

        // Truncated modified Gumbel
        gumbelTruncatedModified::gumbelTruncatedModified(const std::string& s, const double p1, const double p2, const double p3, const double p4) :
            gm(gumbelModified(s, p1, p2, p3)), d(p4)
        {
            if (d <= 0) { throw probLibException("parameter 4 in gumbelTruncatedModified must be > 0"); }
        }

        double gumbelTruncatedModified::getValue(const double u) const
        {
            double p = conversions::PfromBeta(u);
            p = p * (1.0 - d);
            double uu = conversions::betaFromQ(1.0 - p);
            return gm.getValue(uu);
        }
    }
}
