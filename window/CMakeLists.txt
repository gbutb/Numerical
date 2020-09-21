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
    window
    window.cpp
    camera.cpp
    shader/shader.cpp
    shader/mapShader/mapShader.cpp)

target_link_libraries(
    window
    glfw
    GLEW::glew
    ${OPENGL_gl_LIBRARY})
