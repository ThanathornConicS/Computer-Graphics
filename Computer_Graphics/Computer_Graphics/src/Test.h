#ifndef TEST_H
#define TEST_H

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
	vlr::Shader m_shader;

};

#endif // !TEST_H

