#pragma once

#include <pch.h>

struct Light 
{
	float position[3] = {1.0f, -1.0f, 0.0f};
	float direction[3] = { 0.0f, 1.0f, -1.0f };
	float color[3] = { 1.0f, 1.0f , 1.0f };
	float ambientStrength = 1.0f;
	float diffuseStrength = 1.0f;
	float specular[3] = { 1.0f, 1.0f, 1.0f };

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;

	float cutOff = 12.5f;
	float outterCutOff = 17.5f;
};
