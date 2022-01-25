#pragma once

#include <pch.h>
#include "VLRCore.h"

const int timeMax = 4;
const float g = 9.81f;
const float vNull = 0.0f;
const float xNull = 100.0f;

inline void DiiferentialEquation() 
{
	// v = at + vNull
	// x = 1/2(at^2) + (vNull * t) + xNull

	float v, x;

	for (int t = 0; t < timeMax; t++) 
	{
		v = -g * t + vNull;
		x = (0.5f * -g * t * t) + (vNull * t) + xNull;

		L_TRACE("Diff: {0}: v = {1}		x = {2}", t, v, x);
	}
}
inline void EulerIntegration() 
{
	int t = 0, n = 0;
	float v = vNull, x = xNull;

	while (t < timeMax) 
	{
		if (t == timeMax)
			break;

		v = v + g * 1;
		x = x + v * 1;


	}
}
