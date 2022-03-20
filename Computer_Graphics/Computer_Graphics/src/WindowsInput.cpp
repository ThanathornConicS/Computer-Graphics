#include <pch.h>
#include "Input.h"

#include "Window.h"
#include "GLFW/glfw3.h"

namespace vlr
{
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsKeyReleased(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_RELEASE;
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}
	bool Input::IsMouseButtonReleased(int button)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_RELEASE;
	}

	glm::vec2 Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return glm::vec2(xPos, yPos);
	}

	float Input::GetMouseX()
	{
		glm::vec2 pair = GetMousePosition();
		return pair.x;
	}
	float Input::GetMouseY()
	{
		glm::vec2 pair = GetMousePosition();
		return pair.y;
	}
}