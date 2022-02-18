#pragma once

inline double DefiniteIntegral(double (*f)(double), int upperBound, int lowerBound, float stepSize) 
{
	size_t steps = (upperBound - lowerBound) / stepSize;

	double result = 0;
	for (size_t i = 0; i < steps; i++) 
	{
		result += f(static_cast<double>(lowerBound + (i + 0.5) * stepSize)) * stepSize;
	}

	return result;
}
