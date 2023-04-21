#pragma once

#include <pch.h>

/* ImGui */
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

typedef void(*_FUNC)();

class ImguiLayer 
{
public:
	ImguiLayer(void)
		: m_window(nullptr)
	{
		SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	ImguiLayer(GLFWwindow* win)
		: m_window(win)
	{
		SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	~ImguiLayer() {}

	void Init();
	void Render(int w, int h);
	void Terminate();

	void GUI_Begin();
	void GUI_End();

	void RenderDocingSpace();

	void SetScreenSize(int w, int h);
	void SetGLFWwindow(GLFWwindow* window);

	void SetFunction(_FUNC func);
	void ResetFunction();

private:
	int m_width, m_height;
	GLFWwindow* m_window;
	_FUNC m_function = nullptr;
};
