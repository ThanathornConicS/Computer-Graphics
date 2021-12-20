#ifndef SCENE_H
#define SCENE_H

class Scene 
{
public:
	Scene(const char* debugName)
		: m_debugName(debugName) 
	{}
	virtual ~Scene() {}

	virtual void OnAttach() {}
	virtual void OnDeAttach() {}

	virtual void OnUpdate() {}

protected:
	const char* m_debugName = "Default_Scene";

private:


};

#endif // !SCENE_H
