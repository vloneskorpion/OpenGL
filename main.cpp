//OpenGl libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Standard libs
#include <string>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

//Includes
#include "Logger.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "ErrorHandle.hpp"

struct ShaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main()
{
    Logger logger{"../dependencies/easylogging/configuration.conf"};

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
    if(window == NULL)
    {
        LOG(ERROR) << "Couldn't create GLFW window";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(ERROR) << "Failed to initialize GLAD";
        return -1;
    }

    auto positions = std::array<float, 8>{
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    auto indicies = std::array<uint32_t, 6>{
        0, 1, 2,
        2, 3, 0
    };

    auto vao = uint32_t{};

    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    auto vertexBuffer = VertexBuffer{&positions, positions.size() * sizeof(float)};

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0));

    auto indexBuffer = IndexBuffer{(const uint32_t*)&indicies, 6};
    
    const auto shaderSource = ParseShader("../shaders/shader.shader");
    const auto shader = CreateShader(shaderSource.vertexSource, shaderSource.fragmentSource);
    GLCall(glUseProgram(shader));

    auto location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        auto r = sin(glfwGetTime());
        auto b = cos(glfwGetTime());
        GLCall(glUniform4f(location, r, b, 0.8f, 1.0f));
        GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    GLCall(glfwTerminate());
    return 0;
}