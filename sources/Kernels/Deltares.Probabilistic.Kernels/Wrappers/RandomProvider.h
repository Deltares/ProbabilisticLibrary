#pragma once

public delegate void ManagedInitializeRandomDelegate(int seed);
public delegate double ManagedNextRandomDelegate();

ref class RandomProvider
{
private:
	static System::Random^ random = gcnew System::Random();

	static void initializeInitializeDelegate();
	static void initializeNextDelegate();
	static bool initialized = false;

	static void initialize(int seed)
	{
		random = gcnew System::Random(seed);
	}

	static double next()
	{
		return random->NextDouble();
	}

public:
	static void initialize();
};

