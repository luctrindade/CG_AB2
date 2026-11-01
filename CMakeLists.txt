cmake_minimum_required(VERSION 3.10)
project(Cena3D CXX)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE FONTES src/*.cpp)
add_executable(cena main.cpp ${FONTES})
target_include_directories(cena PRIVATE src)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
target_link_libraries(cena OpenGL::GL OpenGL::GLU GLUT::GLUT)
