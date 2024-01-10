#pragma once
#include "RandomValueGenerator.h"

typedef void(__stdcall* InitializeRandomDelegate) (int seed);

typedef double(__stdcall* NextRandomDelegate) ();

class ModifiedKnuthSubtractiveRandomValueGenerator : public RandomValueGenerator
{
private:
	inline static InitializeRandomDelegate initializeDelegate = nullptr;
	inline static NextRandomDelegate nextDelegate = nullptr;
public:
	void initialize(int seed, int seedB) override;
	double next() override;

	static void setInitializeDelegate(InitializeRandomDelegate initializeDelegate)
	{
		ModifiedKnuthSubtractiveRandomValueGenerator::initializeDelegate = initializeDelegate;
	}

	static void setNextDelegate(NextRandomDelegate nextDelegate)
	{
		ModifiedKnuthSubtractiveRandomValueGenerator::nextDelegate = nextDelegate;
	}
};

