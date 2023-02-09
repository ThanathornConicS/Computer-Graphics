#pragma once

#include "Window.h"
#include "WindowsInput.h"
#include "Time.h"

#include "imguiLayer.h"

#include "ShaderManager.h"
#include "SceneManager.h"

namespace vlr
{
	class Application
	{
	public:
		Application();
		~Application();

		void Init();
		void Run();
		void Clean();

		inline GLFWwindow* GetWindow() const
		{
			return m_systemWindow->GetWindow();
		}

		static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

	private:
		void InitWindow();

	private:
		static Window* m_systemWindow;
		SceneManager* m_sceneManager;
		Time m_time;

		ImguiLayer m_imguiLayer;

		Scene* m_currentScene;

		WindowsInput m_input;
	};
}



