#pragma once

class RandomValueGenerator
{
public:
	virtual void initialize(int seed, int seedB) {};
	virtual double next() { return 0; };
};


