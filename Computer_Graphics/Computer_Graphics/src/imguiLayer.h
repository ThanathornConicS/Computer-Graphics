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
		: m_width(600), m_height(600), m_window(nullptr)
	{}
	ImguiLayer(int w, int h, GLFWwindow* win)
		:m_width(w), m_height(h), m_window(win)
	{}
	~ImguiLayer() {}

	void Init();
	void Render(int w, int h);
	void Terminate();

	void GUI_Begin();
	void GUI_End();

	void SetScreenSize(int w, int h);

	void SetFunction(_FUNC func);
	void ResetFunction();

private:
	int m_width, m_height;
	GLFWwindow* m_window;
	_FUNC m_function = nullptr;
};
