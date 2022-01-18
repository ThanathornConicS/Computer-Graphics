#ifndef HDRBUFFER_H
#define HDRBUFFER_H

#include "Framebuffer.h"

class HDRBuffer : public Framebuffer
{
public:
	void CreateBuffer() override;
	void BindBuffer() override;

	void RenderBuffer();

private:
	void AttachBuffer() override;

};

#endif // !HDRBUFFER_H

