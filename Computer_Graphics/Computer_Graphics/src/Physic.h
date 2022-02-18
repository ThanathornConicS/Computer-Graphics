#pragma once

#include <pch.h>
#include "VLRCore.h"

const int timeMax = 6;
const float g = 10;
const float vNull = 0.0f;
const float xNull = 100.0f;

const glm::vec2 gravityVec = glm::vec2(0.0f, -g);
const glm::vec2 vNullVec = glm::vec2(10.0f, 0.0f);
const glm::vec2 xNullVec = glm::vec2(0.0f, 100.0f);

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
template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
inline void EulerIntegration(T h) 
{
	int t = 0, n = 0;
	float v = vNull, x = xNull;

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

inline void NoAirResistance(float h = 1) 
{
	int t = 0, n = 0;
	glm::vec2 v = vNullVec, x = xNullVec;

	while (t < timeMax)
	{
		L_TRACE("NoAir: {0}: v = [{1}, {2}]\t\tx = [{3}, {4}]", t, v.x, v.y, x.x, x.y);
		if (t == timeMax)
			break;

		glm::vec2 tempV = v;
		v = (v + gravityVec) * h;
		x = (x + tempV) * h;

		n = n + 1; t = n * h;
	}
}
inline void WithAirResistance(float dragCoeff = 0.4f, float mass = 1, float h = 1)
{
	int t = 0, n = 0;
	glm::vec2 v = vNullVec, x = xNullVec;
	glm::vec2 a = gravityVec;

	while (t < timeMax)
	{
		L_TRACE("WithAir: {0}: v = [{1}, {2}]\t\tx = [{3}, {4}]", t, v.x, v.y, x.x, x.y);
		if (t == timeMax)
			break;

		glm::vec2 tempV = v;
		a = gravityVec - (dragCoeff / mass) * tempV;
		v = (v + a) * h;
		x = (x + tempV) * h;


		n = n + 1; t = n * h;
	}
}
inline void WindAirResistance(glm::vec2 windVelocity = glm::vec2(-12.5f, 0.0f), float dragCoeff = 0.4f, float mass = 1, float h = 1)
{
	int t = 0, n = 0;
	glm::vec2 v = vNullVec, x = xNullVec;
	glm::vec2 a = gravityVec;

	while (t < timeMax)
	{
		L_TRACE("WindAir: {0}: v = [{1}, {2}]\t\tx = [{3}, {4}]", t, v.x, v.y, x.x, x.y);
		if (t == timeMax)
			break;

		glm::vec2 tempV = v;
		a = gravityVec + (dragCoeff / mass) * (windVelocity - tempV);
		v = (v + a) * h;
		x = (x + tempV) * h;


		n = n + 1; t = n * h;
	}
}
