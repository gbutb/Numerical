# numerical/CMakeLists.txt
# Load OpenGL
find_package(glfw3 3.1 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

# Load includes
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS})

add_library(
    numerical
    context.cpp
    numerical.cpp
    program/program.cpp
    program/heat/heat.cpp)
target_link_libraries(
    numerical
    glfw
    GLEW::glew
    window
    "-framework OpenCL")
