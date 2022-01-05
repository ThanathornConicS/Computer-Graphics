#include "pch.h"
#include "App.h"

#include "RayMarchScene.h"

App::App()
{}
App::~App()
{}

#pragma region Public_Member_Function

void App::Init()
{
	InitWindow();

	m_scenes.resize(MAX_SCENE);

	m_scenes.emplace(m_scenes.begin(), new RayMarchScene());

	m_time.time_last = glfwGetTime();

	m_currentScene = m_scenes[0];
	m_currentScene->OnAttach();
}
void App::Run()
{
	glClearColor(0.6196f, 0.9333f, 0.9451f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_time.ProcessTime(m_systemWindow->GetWindow(), glfwGetTime());

	/* Draw Begin */

	m_currentScene->OnUpdate(m_systemWindow->GetWindow(), m_time);

	/* Draw End */

	// Swap in the back buffer
	glfwSwapBuffers(m_systemWindow->GetWindow());
	glfwPollEvents();
	m_time.current_time += m_time.deltaTime;
}
void App::Clean()
{
	m_currentScene->OnDeAttach();

	m_scenes.clear();
}

#pragma endregion

#pragma region Private_Member_Function

void App::InitWindow()
{
	m_systemWindow = &Window::GetInstance();
	if (!m_systemWindow->GetInitStatus())
		throw std::runtime_error("Cannot initialize window and its context");

	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	std::cout << "                            |--System Initialized--|                            " << std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	std::cout << "                          Version: " << glGetString(GL_VERSION)					<< std::endl;
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
}

#pragma endregion