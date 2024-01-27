#pragma once
#include "RandomValueGenerator.h"

#ifdef _WIN32
typedef void(__stdcall* InitializeRandomDelegate) (bool repeatable, int seed);
typedef double(__stdcall* NextRandomDelegate) ();
#else
typedef void(* InitializeRandomDelegate) (bool repeatable, int seed);
typedef double(* NextRandomDelegate) ();
#endif

class ModifiedKnuthSubtractiveRandomValueGenerator : public RandomValueGenerator
{
private:
	inline static InitializeRandomDelegate initializeDelegate = nullptr;
	inline static NextRandomDelegate nextDelegate = nullptr;
public:
	void initialize(bool repeatable, int seed, int seedB) override;
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

