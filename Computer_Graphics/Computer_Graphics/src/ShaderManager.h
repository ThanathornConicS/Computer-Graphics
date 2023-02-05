#pragma once
#include <pch.h>
#include "Shader.h"

namespace vlr 
{
	class ShaderManager 
	{
	public:
		inline static ShaderManager& GetInstance()
		{
			if (!s_instance)
				s_instance = new ShaderManager();

			return *s_instance;
		}
		virtual ~ShaderManager() {}

		ShaderManager(const ShaderManager& otherInstance) = delete;
		void operator=(ShaderManager& otherInstance) = delete;

		void AddShader(std::string& shaderName, Shader* shaderToAdd);
		void RemoveShader(std::string& shaderNameToRemove);

		Shader* FindShader(std::string shaderNameToFind);

		void ListAll();

	protected:
		ShaderManager();

	private:
		static ShaderManager* s_instance;

		std::unordered_map<std::string, Shader*> m_shaders;
		int sceneIdx = -1;
	};
}
