#include "Shader.hpp"
#include "ErrorHandle.hpp"
#include "easylogging/easylogging++.h"

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <array>

Shader::Shader(const std::string& filepath) :
    m_RendererId{0},
    m_FilePath{filepath}
{
    const auto shaderSource = ParseShader(filepath);
    m_RendererId = CreateShader(shaderSource.vertexSource, shaderSource.fragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererId));
}

uint32_t Shader::CompileShader(uint32_t type, const std::string& source)
{
    auto id = glCreateShader(type);
    auto src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    auto result = int{};
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if(result == GL_FALSE)
    {
        auto lenght = int{};
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
        auto message = static_cast<char*>(alloca(lenght * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, lenght, &lenght, message)); 
        LOG(ERROR) << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? 
        "GL_VERTEX_SHADER " : "GL_FRAGMENT_SHADER ") << message;
        GLCall(glDeleteShader(id));

        return 0;
    }

    return id; 
}

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    auto program = glCreateProgram();
    auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    auto stream = std::ifstream{filepath};

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    auto line = std::string{};
    auto ss = std::array<std::stringstream, 2>{};
    auto type = ShaderType{ShaderType::NONE};

    while(getline(stream, line))
    {
        if(line.find("shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss.at(static_cast<int>(type)) << line << '\n';
        }
    }

    return {ss.at(static_cast<int>(ShaderType::VERTEX)).str(),
            ss.at(static_cast<int>(ShaderType::FRAGMENT)).str()};
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache.at(name);
    }
    auto location = glGetUniformLocation(m_RendererId, name.c_str());

    if(location == -1)
    {
         LOG(ERROR) << "Warning: uniform '" << name << "' doesn't exist!";
         ASSERT(false);
    }

    m_UniformLocationCache[name] = location;
    return location;
}