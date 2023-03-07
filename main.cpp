//OpenGl libs
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

//Standard libs
#include <array>

//Includes
#include "Logger.hpp"
#include "ErrorHandle.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include <imgui/imgui_impl_glfw_gl3.h>

int main()
{
    auto logger = Logger{"../dependencies/easylogging/configuration.conf"};

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

    auto positions = std::array<float, 16>{
        100.0f, 100.0f, 0.0f, 0.0f,
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f,
    };

    auto indicies = std::array<uint32_t, 6>{
        0, 1, 2,
        2, 3, 0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    auto vertexArray = VertexArray{};
    auto vertexBuffer = VertexBuffer{&positions, positions.size() * sizeof(float)};
    auto vertexBufferLayout = VertexBufferLayout{};

    vertexBufferLayout.Push<float>(2);
    vertexBufferLayout.Push<float>(2);
    vertexArray.AddBuffer(vertexBuffer, vertexBufferLayout);

    auto indexBuffer = IndexBuffer{(const uint32_t*)&indicies, 6};

    auto proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    auto view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

    auto shader = Shader{"../shaders/shader.shader"};
    shader.Bind();

    auto texture = Texture{"../resources/textures/texture.jpg"};
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    vertexArray.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();
    shader.Unbind();

    auto renderer = Renderer{};

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    auto translation = glm::vec3{200, 200, 0};

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplGlfwGL3_NewFrame();

        auto model = glm::translate(glm::mat4(1.0f), translation);
        auto mvp = proj * view * model;

        shader.Bind();
        shader.SetUniformMatrix4f("u_MVP", mvp);

        renderer.Draw(vertexArray, indexBuffer, shader);

        {
            ImGui::SliderFloat3("Translation X", &translation.x, 0.0f, 960.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        GLCall(glfwSwapBuffers(window));
        GLCall(glfwPollEvents());
    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    GLCall(glfwTerminate());
    return 0;
}
