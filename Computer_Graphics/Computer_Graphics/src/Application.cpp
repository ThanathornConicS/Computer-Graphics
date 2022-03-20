#include "pch.h"
#include "Application.h"

#include "RayMarchScene.h"
#include "Test.h"
#include "Particlefx.h"

namespace vlr
{
	Window* Application::m_systemWindow = nullptr;

	Application::Application()
	{}
	Application::~Application()
	{}

#pragma region Public_Member_Function

	void Application::Init()
	{
		InitWindow();

		m_scenes.push_back(new Test());
		m_scenes.push_back(new RayMarchScene());
		m_scenes.push_back(new Particlefx());

		m_time.time_last = glfwGetTime();

		m_currentScene = m_scenes[0];
		m_currentScene->OnAttach();
	}
	void Application::Run()
	{
		glClearColor(0.2196f, 0.5333f, 0.5451f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_time.ProcessTime(m_systemWindow->GetWindow(), glfwGetTime());

		/* Draw Begin */

		m_currentScene->OnUpdate(m_time);

		/* Draw End */

		// Swap in the back buffer
		glfwSwapBuffers(m_systemWindow->GetWindow());
		glfwPollEvents();
		m_time.current_time += m_time.deltaTime;
	}
	void Application::Clean()
	{
		m_currentScene->OnDetach();

		for (auto scene : m_scenes)
			delete scene;
		m_scenes.clear();

		m_systemWindow->ReleaseInstance();
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