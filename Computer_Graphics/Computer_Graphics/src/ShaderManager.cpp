#include "ShaderManager.h"

namespace vlr
{
	ShaderManager::ShaderManager()
	{}

	void ShaderManager::AddShader(std::string& shaderName, Shader* shaderToAdd)
	{
		m_shaders.insert({shaderName, shaderToAdd});
	}
	void ShaderManager::RemoveShader(std::string& shaderNameToRemove)
	{
		m_shaders.erase(shaderNameToRemove);
	}

	Shader* ShaderManager::FindShader(std::string shaderNameToFind)
	{
		return m_shaders.find(shaderNameToFind)->second;
	}

	void ShaderManager::ListAll()
	{
		int idx = 0;
		for (auto shader : m_shaders) 
		{
			L_SYSTEM_INFO("ShaderIdx {0}: {1}", idx, shader.first);
			idx++;
		}
	}

}