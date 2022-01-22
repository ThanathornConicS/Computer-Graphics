#pragma once

#ifndef H_WINDOW
#define H_WINDOW

#include <pch.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* Initial Value */
static const int width = 1000;
static const int height = 1000;
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

			AddRef();
			//L_SYSTEM_TRACE("Adding\n\tWindow Instance: {0}", m_instanceCounter);

			return *s_instance;
		}
		inline static void ReleaseInstance() 
		{
			RemoveRef();
			//L_SYSTEM_TRACE("Removing\n\tWindow Instance: {0}", m_instanceCounter);

			if (m_instanceCounter == 0 && s_instance) 
			{
				delete s_instance;
				s_instance = nullptr;
			}
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
		static int m_instanceCounter;

		GLFWwindow* m_window = nullptr;
		int m_width, m_height;
		int m_xpos, m_ypos;

		const char* m_windowName;

		bool m_initStatus = true;

		inline static void AddRef() { ++m_instanceCounter; }
		inline static void RemoveRef() { --m_instanceCounter; }
	};
}

#endif // !H_WINDOW
