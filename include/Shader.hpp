#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader
{
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        //Set uniforms
        void SetUniform1i(const std::string& name, int value);
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniform1f(const std::string& name, float value);

        void SetUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

    private:
        int GetUniformLocation(const std::string& name);
        uint32_t CompileShader(uint32_t type, const std::string& source);
        uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        ShaderProgramSource ParseShader(const std::string& filepath);

        uint32_t m_RendererId;
        std::string m_FilePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;
};