#pragma once

#ifndef H_WINDOW
#define H_WINDOW

#include <pch.h>

#include "Utilities.h"

/* Initial Value */
static const int width = 1000;
static const int height = 1000;
static const char* name = "Computer_Graphics";
static const int xpos = 100;
static const int ypos = 100;

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

	void SetWindowSize(int w, int h);
	void SetWindowPos(int x, int y);
	void SetWindowName(const char* n);

	std::pair<int, int> GetWindowSize() const;
	std::pair<int, int> GetWindowPos() const;
	const char* GetWindowName() const;
	GLFWwindow* GetWindow() const;

protected:
	Window();

private:
	static Window* s_instance;
	GLFWwindow* m_window = nullptr;
	int m_width, m_height;
	int m_xpos, m_ypos;

	const char* m_windowName;

};

#endif // !H_WINDOW
