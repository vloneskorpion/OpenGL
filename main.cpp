//Logging libs
#include "easylogging++.h"
INITIALIZE_EASYLOGGINGPP

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

//Includes

int main()
{
    //Logger
    el::Configurations loggerConfiguration("../dependencies/easylogging/configuration.conf");
    el::Loggers::reconfigureAllLoggers(loggerConfiguration);

    LOG(INFO) << "WORKING!";
    return 0;
}