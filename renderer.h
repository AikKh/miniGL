#pragma once

#include "shader.h"

class Renderer {
public:
	Renderer(const char* vertexPath, const char* fragmentPath) : m_shader{ vertexPath, fragmentPath } {}

#pragma region Setters

    void SetBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(m_shader.GetID(), name.c_str()), (int)value);
    }

    void SetInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(m_shader.GetID(), name.c_str()), value);
    }

    void SetFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(m_shader.GetID(), name.c_str()), value);
    }

    void SetVec2(const std::string& name, float x, float y)
    {
        glUniform2f(glGetUniformLocation(m_shader.GetID(), name.c_str()), x, y);
    }

    void SetVec3(const std::string& name, float x, float y, float z) 
    { 
        glUniform3f(glGetUniformLocation(m_shader.GetID(), name.c_str()), x, y, z); 
    }

    void SetVec3(const std::string& name, const glm::vec3& vec)
    {
        glUniform3f(glGetUniformLocation(m_shader.GetID(), name.c_str()), vec.x, vec.y, vec.z);
    }

    void SetMatrix3f(const std::string& name, const glm::mat3& value)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void SetMatrix4f(const std::string& name, const glm::mat4& value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shader.GetID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

#pragma endregion

    void Activate() const
    {
        m_shader.Bind();
    }

private:
	Shader m_shader;
};