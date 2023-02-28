cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

set(PROJECT_NAME opengl)

project(${PROJECT_NAME}  VERSION 1.0)

file(GLOB_RECURSE OPENGL_SOURCES src/*.cpp src/*.c)

add_library(logging_lib STATIC ${CMAKE_SOURCE_DIR}/dependencies/easylogging/easylogging++.cc)
add_executable(${PROJECT_NAME} main.cpp ${OPENGL_SOURCES})

find_package(glfw3 3.4 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

target_include_directories(logging_lib PUBLIC ${CMAKE_SOURCE_DIR}/dependencies/easylogging)
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dependencies)

target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
target_link_libraries(${PROJECT_NAME} "-framework IOKit")
target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} logging_lib)