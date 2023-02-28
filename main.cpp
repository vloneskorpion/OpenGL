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

//Includes
#include "Logger.hpp"

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    auto id = glCreateShader(type);
    auto src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    auto result = int{};
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        auto lenght = int{};
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
        auto message = static_cast<char*>(alloca(lenght * sizeof(char)));
        glGetShaderInfoLog(id, lenght, &lenght, message); 
        LOG(ERROR) << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? 
        "GL_VERTEX_SHADER " : "GL_FRAGMENT_SHADER ") << message;
        glDeleteShader(id);

        return 0;
    }

    return id; 
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    auto program = glCreateProgram();
    auto vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    auto fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

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

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(ERROR) << "Failed to initialize GLAD";
        return -1;
    }

    float positions[] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    auto buffer = uint32_t{};
    auto VAO = uint32_t{};
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    const auto vertexShader = std::string
        {"#version 330 core\n"
         "\n"
         "layout(location = 0) in vec4 position;\n"
         "\n"
         "void main()\n"
         "{\n"
            "gl_Position = position;\n"
         "}\n"};

    const auto fragmentShader = std::string
        {"#version 330 core\n"
         "\n"
         "out vec4 color;\n"
         "\n"
         "void main()\n"
         "{\n"
            "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
         "}\n"};


    const auto shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
 

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}