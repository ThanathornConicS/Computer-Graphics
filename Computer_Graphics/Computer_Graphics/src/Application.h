#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Input.h"
#include "Time.h"

#include "Scene.h"

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
		Time m_time;

		std::vector<Scene*> m_scenes;
		Scene* m_currentScene;
		int m_sceneAmount = 0;
	};
}

#endif // !APP_H


