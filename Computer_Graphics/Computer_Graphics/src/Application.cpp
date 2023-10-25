#include "pch.h"

#include "Application.h"

#include "RayMarchScene.h"
#include "Test.h"

namespace vlr
{
	Window* Application::m_systemWindow = nullptr;

	Application::Application()
	{
		m_currentScene = nullptr;
		m_sceneManager = nullptr;
	}
	Application::~Application()
	{}

#pragma region Public_Member_Function

	void Application::Init()
	{
		InitWindow();

		m_imguiLayer.SetGLFWwindow(m_systemWindow->GetWindow());
		m_imguiLayer.Init();

		m_time.time_last = glfwGetTime();

		m_sceneManager = &SceneManager::GetInstance();
		m_sceneManager->AddScene(new Test());
		m_sceneManager->AddScene(new RayMarchScene());

		m_currentScene = m_sceneManager->FindScene(0);
		m_currentScene->OnAttach();
	}
	void Application::Run()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_time.ProcessTime(m_systemWindow->GetWindow(), glfwGetTime());

		/* Draw Begin */

		m_imguiLayer.GUI_Begin();
		m_imguiLayer.RenderDocingSpace();
		m_currentScene->OnUpdate(m_time);
		m_imguiLayer.GUI_End();

		/* Draw End */

		// Swap in the back buffer
		glfwSwapBuffers(m_systemWindow->GetWindow());
		glfwPollEvents();
		m_time.current_time += m_time.deltaTime;
	}
	void Application::Clean()
	{
		m_currentScene->OnDetach();
		m_imguiLayer.Terminate();
	}

#pragma endregion

#pragma region Private_Member_Function

	void Application::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		m_systemWindow->ResizeWindow(width, height);
	}

	void Application::InitWindow()
	{
		m_systemWindow = &Window::GetInstance();

		glfwSetFramebufferSizeCallback(m_systemWindow->GetWindow(), FrameBufferSizeCallback);

		if (!m_systemWindow->GetInitStatus())
			throw std::runtime_error("Cannot initialize window and its context");

		std::cout << "--------------------------------------------------------------------------------" << std::endl;
		std::cout << "                            |--System Initialized--|                            " << std::endl;
		std::cout << "--------------------------------------------------------------------------------" << std::endl;
		std::cout << "                          Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "--------------------------------------------------------------------------------" << std::endl;
	}
}

#pragma endregion