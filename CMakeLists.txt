cmake_minimum_required(VERSION 2.8)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)

PROJECT(final_project)

include_directories(/usr/local/include)
include_directories(include)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS})

add_executable(main src/main.cpp)
target_link_libraries(main ${OPENGL_LIBRARIES} glfw)
