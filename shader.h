#pragma once

#include "bindable.h"
#include "includes.h"

class Shader : public Bindable {
public:
    Shader(const char* vertexPath, const char* fragmentPath);

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void Bind() const override
    {
        glUseProgram(_id);
    }

    void Unbind() const override
    {
        glUseProgram(0);
    }

    GLuint GetID() const
    {
        return _id;
    }

    void Delete() override
    {
        glDeleteProgram(_id);
    }

    virtual ~Shader()
    {
        Delete();
    }

private:
    GLuint CreateShader(GLuint shader_type, const std::string& source);

    bool HandleShaderErrors(GLuint shader, const std::string& message);

    static std::string readFile(const char* path);

private:
    GLuint _id;
};