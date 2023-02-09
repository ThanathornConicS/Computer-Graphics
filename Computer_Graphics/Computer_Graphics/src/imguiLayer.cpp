#include "pch.h"
#include "imguiLayer.h"

#include <GL/glew.h>
#include <GLFw/glfw3.h>

void ImguiLayer::Init()
{
	//Setup Dear ImGui Context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			//Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			//Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;				//Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;				//Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;	
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	

	//Setup Dear ImGui Style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	//Setup Platform/Renderer Binding
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}
void ImguiLayer::Render(int w, int h)
{
	SetScreenSize(w, h);

	static bool show = false;
	//ImGui::ShowDemoWindow(&show);

	ImGui::Begin("Debugging Window");
	if (m_function != nullptr) 
	{
		m_function();
	}
	ImGui::End();
}
void ImguiLayer::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImguiLayer::GUI_Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void ImguiLayer::GUI_End()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(m_width), static_cast<float>(m_height));

	//Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImguiLayer::SetScreenSize(int w, int h)
{
	m_width = w; m_height = h;
}
void ImguiLayer::SetGLFWwindow(GLFWwindow* window) 
{
	m_window = window;
}

void ImguiLayer::SetFunction(_FUNC func)
{
	m_function = func;
}
void ImguiLayer::ResetFunction()
{
	m_function = nullptr;
}
