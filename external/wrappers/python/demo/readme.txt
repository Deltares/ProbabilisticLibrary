Example of running a probabilistic computation with the z-function and definitions in python.

The following distributions are recoqnized:
0 deterministic
1 uniform
2 normal
3 lognormal
4 lognormal2
5 shiftedExponential
6 gumbel
7 gumbel2
8 weibull
9 rayleigh
10 pareto
11 triangular
12 table
13 table (not yet in python)
14 conditionalWeibull
15 modifiedGumbel
16 truncatedModifiedGumbel
17 table (not yet in python)
18 RayleighN
19 truncatedNormal
20 beta
21 uspace

methods:
1 form (first order reliability method)
3 cm (Crude Monte Carlo)
4 ds (Directional Sampling)
5 ni (numerical integration)
6 is (importance sampling)
7 adaptiveIm (adaptive importance sampling)
11 fdir (form + ds)
16 dsfi (ds + form)

rnd:
gm George-Marsaglia
mt Mersenne-Twister
seed one seed for Mersenne-Twister
seed1 first seed for George-Marsaglia
seed2 second seed for George-Marsaglia

settings:
progressInterval
  integer for progress of convergence
varianceFactor
  double value for im only
epsilonDu
  double value for ds only
startmethod:
  zero
  one
  ray-search
  sphere-search

the zfunction is:
  def z(x)
  with x an array holding all x values.
