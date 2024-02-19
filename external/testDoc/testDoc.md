Test results for $ z = 1 - \sqrt{x1^2 + x2^2} $

Compare py2problib with Fortran demo in original prob. lib.

Beta differs per method.
Alpha vector is not defined.

FORM will give beta = 1;
Alpha depends on start method.
start method zero does not work

DS : beta = -0.270288
Alpha depends on seed.
DS converges very quickly (2 samples are enough).
Due to round off errors, alpha can differ from first value; than differences between py2problib and Fortran demo.
With seed = 1234; minsamples < 15 results are the same for py2problib and Fortran demo.

Test results for $ z = 2 - \sqrt{x1^2 + x2^2} $
DS : beta (Fortran) = 1.1015195611256401
     beta (DS)      = 1.101520
And alphas are the same
