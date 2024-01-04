#pragma once
class StandardNormal
{
private:
	static void normp(const double z, double& p, double& q, double& pdf);

public:
	static double getPFromU(double u);
	static double getQFromU(double u);
	static double getUFromQ(double u);
	static double BetaMax;
};

