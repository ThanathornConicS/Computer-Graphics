#pragma once

#include "Scene.h"

class Test : public vlr::Scene
{
public:
	Test();
	virtual ~Test();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(vlr::Time time) override;

private:
	void ProcessInput();

private:
	vlr::Shader m_normalShader;
	vlr::Shader m_hdrShader;

	HDRBuffer m_hdrBuffer;

	float m_exposure = 1.0f;

};

