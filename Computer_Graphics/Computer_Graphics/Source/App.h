#ifndef APP_H
#define APP_H

#include "Window.h"
#include "Time.h"

#include "Scene.h"

class App
{
public:
	App();
	~App();

	void Init();
	void Run();
	void Clean();

	inline GLFWwindow* GetWindow() const 
	{
		return m_systemWindow->GetWindow();
	}

private:
	void InitWindow();

private:
	Window* m_systemWindow;
	Time m_time;

	std::vector<Scene*> m_scenes;
	Scene* m_currentScene;
	int m_sceneAmount = 0;
};

#endif // !APP_H


