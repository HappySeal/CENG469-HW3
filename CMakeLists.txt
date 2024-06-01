cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 14)

# Add the headers directory to the include path
include_directories(headers)
include_directories(sources)

# Collect all source files from the sources directory
file(GLOB sources "sources/*.cpp")

# Define the executable
add_executable(OpenGLProject ${SOURCES})

# Assuming you are using GLFW and GLEW
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Link the libraries
target_link_libraries(OpenGLProject glfw)
target_link_libraries(OpenGLProject GLEW::GLEW)