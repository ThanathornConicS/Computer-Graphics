#pragma once

#include <pch.h>

#include "GL/glew.h"
#include "glm.hpp"

namespace vlr
{
    class Shader
    {
    public:
        uint32_t ID;
        // constructor generates the shader on the fly
        // ------------------------------------------------------------------------
        Shader(void);
        Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        // activate the shader
        void Compile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
        void Use();
        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;

        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec2(const std::string& name, float x, float y) const;

        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;

        void SetVec4(const std::string& name, const glm::vec4& value) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w);

        void SetMat2(const std::string& name, const glm::mat2& mat) const;
        void SetMat3(const std::string& name, const glm::mat3& mat) const;
        void SetMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        // utility function for checking shader compilation/linking errors.
        // ------------------------------------------------------------------------
        inline void CheckCompileErrors(GLuint shader, std::string type)
        {
            GLint success;
            GLchar infoLog[1024];
            if (type != "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
}