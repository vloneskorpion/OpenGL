//OpenGl libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Standard libs
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
#include "VertexArray.hpp"
#include "Shader.hpp"

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

    auto vertexArray = VertexArray{};
    auto vertexBuffer = VertexBuffer{&positions, positions.size() * sizeof(float)};
    auto vertexBufferLayout = VertexBufferLayout{};

    vertexBufferLayout.Push<float>(2);
    vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

    auto indexBuffer = IndexBuffer{(const uint32_t*)&indicies, 6};
    
    Shader shader("../shaders/shader.shader");
    shader.Bind();

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        auto r = sin(glfwGetTime());
        auto b = cos(glfwGetTime());
        shader.SetUniform4f("u_Color", r, 1.0, b, 1.0);
        GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());
    }

    GLCall(glfwTerminate());
    return 0;
}