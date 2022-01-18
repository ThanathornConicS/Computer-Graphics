#ifndef RAYMARCHSCENE_H
#define RAYMARCHSCENE_H

#include "Scene.h"

class RayMarchScene : public vlr::Scene
{
public:
	RayMarchScene();
	virtual ~RayMarchScene();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(vlr::Time time) override;

private:
	void ProcessInput();

private:
	vlr::Shader m_skyBoxShader;
	vlr::Shader m_rayMarchShader;
	vlr::Shader m_hdrShader;

	float m_rotRate = 1.0f;
	float m_exposure = 1.0f;

	vlr::WindowsInput m_input;
};

#endif // !RAYMARCHSCENE_H
