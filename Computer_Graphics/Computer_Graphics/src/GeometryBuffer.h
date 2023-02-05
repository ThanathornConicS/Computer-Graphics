#pragma once

#include "Framebuffer.h"

class GeometryBuffer : public Framebuffer
{
public:
	void CreateBuffer() override;
	void BindBuffer() override;

	void RenderBuffer();

	uint32_t gPosition, gNormal, gAlbedoSpec;

private:
	void AttachBuffer() override;

};


