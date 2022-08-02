#pragma once

#include <pch.h>
#include <GL/glew.h>

class Framebuffer 
{
public:
	Framebuffer() 
		:m_framebuffer(0), m_colorBuffer(0), m_renderBuffer(0)
	{}
	virtual ~Framebuffer() {}

	virtual void CreateBuffer() {}
	virtual void AttachBuffer() {}
	virtual void BindBuffer() {}

	uint32_t m_framebuffer;
	uint32_t m_colorBuffer;
	uint32_t m_renderBuffer;
};


