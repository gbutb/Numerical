/**
 * window.cpp
 * ----------
 * 
 * Implements methods in window.hpp
 */

#include "window.hpp"


Window::Window(string window_title, int width, int height) :
        _width(width), _height(height) {
    
    // Initialize glfw
    if (!glfwInit())
        throw std::runtime_error("Unable to initialize GLFW");

    // Create window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Initialize GLFW window
    _window = glfwCreateWindow(
        width, height, window_title.c_str(),
        nullptr, nullptr);
    if (_window == nullptr)
        throw std::runtime_error("Unable to initialize GLFW window");

    // Configure context
    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, this);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
        throw std::runtime_error("Unable to initialize GLEW");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(_window,
        [] (GLFWwindow * win, double xpos, double ypos) {
            auto gui = reinterpret_cast<Window*>(
                glfwGetWindowUserPointer(win));
            static float lastX = gui->getWidth() / 2;
            static float lastY = gui->getHeight() / 2;

            // Compute offsets
            float xoffset = xpos - lastX;
            float yoffset = lastY - ypos;

            float sensitivity = .2f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            if (gui->getCamera() != nullptr)
                gui->getCamera()->rotate(xoffset, yoffset);
        });
}

Window::~Window() {
    // Terminate GLFW
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void Window::attachCamera(ptr_Camera camera) {
    _camera = camera;
}


Window::operator bool() {
    // Render and poll
    glfwSwapBuffers(_window);
    glfwPollEvents();

    // Clear color and depth buffers
    glClearColor(.1f, .1f, .1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _width, _height);

    GLenum err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        printf("Error: 0x0%x\n", err);
    }

    _handleKeys();

    // TODO: Render shaders

    return !glfwWindowShouldClose(_window);
}

int Window::getWidth() const {
    return _width;
}

int Window::getHeight() const {
    return _height;
}

ptr_Camera Window::getCamera() const {
    return _camera;
}

void Window::_handleKeys() {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, true);

    // Camera controls
    if (_camera == nullptr) return;
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera->translate(glm::vec3(1, 0, 0));

    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera->translate(glm::vec3(-1, 0, 0));

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera->translate(glm::vec3(0, 1, 0));

    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera->translate(glm::vec3(0, -1, 0));

    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        _camera->translate(glm::vec3(0, 0, 1));

    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        _camera->translate(glm::vec3(0, 0, -1));
}
