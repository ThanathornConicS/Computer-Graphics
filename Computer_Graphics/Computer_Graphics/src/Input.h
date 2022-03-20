#ifndef INPUT_H
#define INPUT_H

#include <pch.h>

#include "glm.hpp"
#include "KeyCode.h"
#include "MouseCode.h"

namespace vlr
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsKeyReleased(int keycode);

		static bool IsMouseButtonPressed(int button);
		static bool IsMouseButtonReleased(int button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

#endif // !INPUT_H