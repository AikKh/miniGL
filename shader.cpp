#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    auto vertexSource = readFile(vertexPath);
    auto fragmentSource = readFile(fragmentPath);

    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentSource);

    if (!HandleShaderErrors(vertexShader, "Vertex error") || !HandleShaderErrors(fragmentShader, "Fragment error"))
    {
        return;
    }

    _id = glCreateProgram();
    glAttachShader(_id, vertexShader);
    glAttachShader(_id, fragmentShader);
    glLinkProgram(_id);

    if (!HandleShaderErrors(_id, "Program error"))
    {
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    Bind();
}

GLuint Shader::CreateShader(GLuint shader_type, const std::string& source)
{
    GLuint shader = glCreateShader(shader_type);

    auto str = source.c_str();

    glShaderSource(shader, 1, &str, NULL);
    glCompileShader(shader);

    return shader;
}

bool Shader::HandleShaderErrors(GLuint shader, const std::string& message)
{
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << message << ": " << infoLog << std::endl;
        return false;
    }

    return true;
}

std::string Shader::readFile(const char* path)
{
    std::ifstream file(path); // Create an input file stream
    if (!file.is_open())
    {
        std::cerr << "Could not open the file!" << std::endl;
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
        content += line + "\n"; // Add each line and a newline character to the content string
    }

    file.close(); // Close the file
    return content;
}
