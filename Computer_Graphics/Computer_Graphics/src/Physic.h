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

		L_TRACE("Diff: {0}: v = {1}\t\tx = {2}", t, v, x);
	}
}
inline void EulerIntegration(float h = 1) 
{
	int t = 0, n = 0;
	float v = vNull, x = xNull;

	//L_TRACE("Euler: {0}: v = {1}\t\tx = {2}", t, v, x);
	while (t < timeMax) 
	{
		L_TRACE("Euler: {0}: v = {1}\t\tx = {2}", t, v, x);
		if (t == timeMax)
			break;

		float tempV = v;
		v = v + -g * h;
		x = x + tempV * h;

		n = n + 1; t = n * h;
	}
}
