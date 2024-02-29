#include "GeorgeMarsagliaRandomValueGenerator.h"

#include <cmath>

#include "RandomValueGenerator.h"
#include "../../Utils/probLibException.h"

//> @file
// This file contains the random number generator proposed by George Marsaglia in
// Florida State University Report: FSU-SCRI-87-50
//<
//
//>
// This module contains a type for the random number generator and two methods
//
// I.e. the subroutine to initialize the random number generator and the method to generate a random value
// (See Numerical Recipes, chapter 7)
//
//>
// This subroutine contains the initialization routine for the random number generator
//
// <i>NOTE</i> \n
// The seed variables can have values between:
// \li 0 <= ij <= 31328
// \li 0 <= kl <= 30081
//
// The random number sequences created by these two seeds are of sufficient
// length to complete an entire calculation with. For example, if several
// different groups are working on different parts of the same calculation,
// each group could be assigned its own IJ seed. This would leave each group
// with 30000 choices for the second seed. That is to say, this random
// number generator can create 900 million different subsequences -- with
// each subsequence having a length of approximately 10^30.
//

namespace Deltares
{
    namespace Numeric
    {
        void GeorgeMarsagliaRandomValueGenerator::initialize(bool repeatable, const int ij, const int kl)
        {
            //
            // Verify the two seeds
            //
            if (ij < 0 || ij > 31328)
            {
                throw Deltares::ProbLibCore::probLibException("Random seed parameter 1 outside of valid range (0 ... 31328)", ij);
            }
            if (kl < 0 || kl > 30081)
            {
                throw Deltares::ProbLibCore::probLibException("Random seed parameter 2 outside of valid range (0 ... 30081)", kl);
            }

            gm_state& state = GeorgeMarsagliaRandomValueGenerator::state;
            //
            // Initialize the parameters of the random number generator
            //
            int i = (ij / 177) % 177 + 2;  // first parameter in the random number generator
            int j = ij % 177 + 2;  // second param
            int k = (kl / 169) % 178 + 1;  // third param
            int l = kl % 169;      // fourth param
            //
            // Generate values to the vector u
            //
            for (int ii = 1; ii <= 97; ii++)
            {
                double s = 0.0;
                double t = 0.5;
                for (int jj = 1; jj <= 24; jj++)
                {
                    int m = ((i * j) % 179 * k) % 179;
                    i = j;
                    j = k;
                    k = m;
                    l = (53 * l + 1) % 169;
                    if ((l * m) % 64 >= 32)
                    {
                        s += t;
                    }
                    t *= 0.5;
                }
                state.u[ii] = s;
            }
            //
            // Set all other arguments of the variable state
            //
            state.c = 362436.0 / 16777216.0;
            state.cd = 7654321.0 / 16777216.0;
            state.cm = 16777213.0 / 16777216.0;
            state.i97 = 97;
            state.j97 = 33;
        }

        //>
        // This method returns a uniform random deviate between 0.0 and 1.0
        double GeorgeMarsagliaRandomValueGenerator::next()
        {
            gm_state& state = GeorgeMarsagliaRandomValueGenerator::state;
            //
            // Generate a uniform random deviate between 0 and 1
            //
            double randomValue = state.u[state.i97] - state.u[state.j97];
            if (randomValue < 0.0)
            {
                randomValue++;
            }
            state.u[state.i97] = randomValue;
            state.i97--;
            if (state.i97 == 0)
            {
                state.i97 = 97;
            }
            state.j97--;
            if (state.j97 == 0)
            {
                state.j97 = 97;
            }
            state.c -= state.cd;
            if (state.c < 0.0)
            {
                state.c += state.cm;
            }
            randomValue -= state.c;
            if (randomValue < 0.0)
            {
                randomValue++;
            }
            return randomValue;
        }
    }
}



