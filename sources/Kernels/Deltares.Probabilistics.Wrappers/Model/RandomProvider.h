#pragma once

public delegate void ManagedInitializeRandomDelegate(bool repeatable, int seed);
public delegate double ManagedNextRandomDelegate();

ref class RandomProvider
{
private:
	static System::Random^ random = gcnew System::Random();

	static void initializeInitializeDelegate();
	static void initializeNextDelegate();
	static bool initialized = false;

	static void initialize(bool repeatable, int seed)
	{
		if (repeatable) 
		{
			random = gcnew System::Random(seed);
		}
		else
		{
			random = gcnew System::Random();
		}
	}

	static double next()
	{
		return random->NextDouble();
	}

public:
	static void initialize();
};

