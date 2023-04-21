#pragma once

#include <pch.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Initial Value */
static const int width = 1280;
static const int height = 720;
static const char* name = "Computer_Graphics";
static const int xpos = 800;
static const int ypos = 100;

namespace vlr
{
	class Window
	{
	public:
		inline static Window& GetInstance()
		{
			if (!s_instance)
				s_instance = new Window();

			return *s_instance;
		}
		virtual ~Window();

		Window(const Window& otherInstance) = delete;
		void operator=(Window& otherInstance) = delete;

		void ResizeWindow(int width, int height);

		void SetWindowSize(int w, int h);
		void SetWindowPos(int x, int y);
		void SetWindowName(const char* n);

		std::pair<int, int> GetWindowSize() const;
		std::pair<int, int> GetWindowPos() const;
		const char* GetWindowName() const;
		GLFWwindow* GetWindow() const;

		inline bool GetInitStatus() const
		{
			return m_initStatus;
		}

	protected:
		Window();

	private:
		static Window* s_instance;
		GLFWwindow* m_window = nullptr;
		int m_width, m_height;
		int m_xpos, m_ypos;

		const char* m_windowName;

		bool m_initStatus = true;

	};
}
