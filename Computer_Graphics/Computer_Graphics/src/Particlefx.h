#pragma once

#include "Scene.h"
#include "VLRCore.h"

class Particlefx : public vlr::Scene
{
public:
	Particlefx();
	virtual ~Particlefx();

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(vlr::Time time) override;

private:
};

