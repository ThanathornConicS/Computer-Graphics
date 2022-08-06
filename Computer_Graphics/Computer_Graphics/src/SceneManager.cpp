#include <pch.h>
#include "SceneManager.h"

namespace vlr 
{
	SceneManager* SceneManager::s_instance = nullptr;

	SceneManager::SceneManager()
	{
		m_scenes.clear();
	}

	void SceneManager::AddScene(Scene& sceneToAdd, std::string& sceneNameToAdd)
	{
		//m_scenes.insert(sceneNameToAdd, sceneToAdd);
	}
	void SceneManager::RemoveScene(std::string& sceneNameToRemove)
	{
		
	}
}