#include <pch.h>
#include "WindowsInput.h"

#include "Window.h"
#include "GLFW/glfw3.h"

namespace vlr
{
	Input* Input::s_instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowsInput::IsKeyReleasedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_RELEASE;
	}
	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	std::pair<double, double> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Window::GetInstance().GetWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { xPos, yPos };
	}

	double WindowsInput::GetMouseXImpl()
	{
		std::pair<double, double> pair = GetMousePositionImpl();
		return pair.first;
	}
	double WindowsInput::GetMouseYImpl()
	{
		std::pair<double, double> pair = GetMousePositionImpl();
		return pair.second;
	}
}