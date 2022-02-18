#pragma once

#include "Scene.h"
#include "VLRCore.h"

#include "ParticleSystem.h"

class Test : public vlr::Scene
{
public:
	Test();
	virtual ~Test();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(vlr::Time time) override;

private:
	vlr::Camera m_camera;
	vlr::Shader m_shader;

};


