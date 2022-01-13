#ifndef RAYMARCHSCENE_H
#define RAYMARCHSCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Time.h"

#include "Scene.h"
#include "Shader.h"

class RayMarchScene : public Scene
{
public:
	RayMarchScene();
	virtual ~RayMarchScene();

	void OnAttach() override;
	void OnDeAttach() override;

	void OnUpdate(GLFWwindow* window, Time time) override;

private:
	void ProcessInput(GLFWwindow* window);
	static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	Shader m_skyBoxShader;
	Shader m_rayMarchShader;
	Shader m_hdrShader;

	float m_rotRate = 1.0f;
	float m_exposure = 1.0f;

};

#endif // !RAYMARCHSCENE_H
