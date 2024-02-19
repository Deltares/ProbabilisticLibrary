enum class combineAndOr
{
    combAnd,
    combOr,
};

extern "C"
int combinemultipleelements_c(double *betaElement, double *alphaElement, double *rho,
  double *beta, double *alpha, combineAndOr combAndOrIn, int nrElms, int nrStoch)
{
    return -999;
}

extern "C"
int hohenbichler_c(double* betaV, double* pfU, double* rhoInput, double* pfVpfU)
{
    return -999;
}

extern "C"
int computebetasection_c(double * betaCrossSection, double* sectionLength, double* breachL, double* rhoZ,
    double* dz, double* deltaL, double* betaSection)
{
    return -999;
}

extern "C"
int upscalelengthc(double* betaCrossSection, double* alphaCrossSection, double* rhoXK, double* dXK,
    double* sectionLength, double* betaSection, double* alphaSection, double* breachLength, int nStochasts)
{
    return -999;
}

extern "C"
int upscaleintimec(double* nrTimes, double* beta, double* alpha, double* inRhoT, int nStochasts)
{
    return -999;
}

extern "C"
void upscaletolargestblockc(double* betaSmallBlock, double* alphaSmallBlock, double* rhoTSmallBlock,
    double* blockDurations, double* largestBlockDuration,
    double* betaLargestBlock, double* alphaLargestBlock, double* durationsLargestBlock, int nStochasts)
{
}

extern "C"
void integrateequalelements(double * beta, double * rhoT, double * nrElements, double * betaT)
{
}

extern "C"
int combinetwoelementspartialcorrelationc1(double* beta1, double* alpha1, double* beta2, double* alpha2, double* rhoP,
    double* betaC, double* alphaC, const int combAndOr, const int nStochasts, double* alphaI, double* alphaII)
{
    return -999;
}

extern "C"
int combinetwoelementspartialcorrelationc2(double* beta1, double* alpha1, double* beta2, double* alpha2, double* rhoP,
    double* betaC, double* alphaC, const int combAndOr, const int nStochasts)
{
    return -999;
}

extern "C"
void getmultipleelementshigestbeta_c(double* betaElement, double* alphaElement,
    double* beta, double* alpha, int nrElms, int nrStoch)
{
}

extern "C"
int combinemultipleelementsspatialcorrelated_c(double* betaElement, double* alphaElement, double* rhoIn,
    double* beta, double* alpha, const combineAndOr combAndOr, int nrElms, int nrStoch)
{
    return -999;
}

extern "C"
int combinemultipleelementsprob_c(double* betaElement, double* alphaElement, double* percentagesIn,
    double* beta, double* alpha, const int combAndOr, int nrElms, int nrStoch)
{
    return -999;
}

