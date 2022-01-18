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


private:

};

#endif // !TEST_H

