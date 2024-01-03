#pragma once
class Sample
{
private:

public:
	Sample() {};

	Sample(int size) {
		Values = new double[size];
		for (int i = 0; i < size; i++) {
			Values[i] = 0;
		}
	}

	double* Values;

	int IterationIndex = -1;
};

