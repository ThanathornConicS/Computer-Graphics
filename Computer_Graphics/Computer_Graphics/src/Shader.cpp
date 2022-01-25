#include "pch.h"
#include "Shader.h"

namespace vlr
{
    Shader::Shader(void)
        : ID(0)
    {}
    Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
        : ID(0)
    {
        Compile(vertexPath, fragmentPath, geometryPath);
    }

    Shader* Shader::CompileFromText(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    {
        Shader* shader = new Shader();
        shader->Compile(vertexPath, fragmentPath, geometryPath);
        return shader;
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void Shader::Compile(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            L_SYSTEM_WARN("Error (code: {0}): {1}", e.code(), e.what());
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryPath != nullptr)
        {
            const char* gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            CheckCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (geometryPath != nullptr)
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        CheckCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }

    void Shader::Use()
    {
        glUseProgram(ID);
    }

    // utility uniform functions
    void Shader::SetBool(const std::string& name, bool value) const
    {
        glUniform1i(FindUniformLocation(name), (int)value);
    }
    void Shader::SetInt(const std::string& name, int value) const
    {
        glUniform1i(FindUniformLocation(name), value);
    }
    void Shader::SetFloat(const std::string& name, float value) const
    {
        glUniform1f(FindUniformLocation(name), value);
    }

    void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(FindUniformLocation(name), 1, &value[0]);
    }
    void Shader::SetVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(FindUniformLocation(name), x, y);
    }

    void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(FindUniformLocation(name), 1, &value[0]);
    }
    void Shader::SetVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(FindUniformLocation(name), x, y, z);
    }

    void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(FindUniformLocation(name), 1, &value[0]);
    }
    void Shader::SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(FindUniformLocation(name), x, y, z, w);
    }

    void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(FindUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(FindUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(FindUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }
    
    GLint Shader::FindUniformLocation(const std::string& uni_name) const 
    {
        if (m_uniformLocationCache.find(uni_name) != m_uniformLocationCache.end())
            return m_uniformLocationCache[uni_name];

        GLint location = glGetUniformLocation(ID, uni_name.c_str());
        m_uniformLocationCache[uni_name] = location;
        return location;
    }
}