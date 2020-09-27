/**
 * window.hpp
 * ----------
 * 
 * Contains wrappers for the window.
 */

#ifndef WINDOW_WINDOW_HPP_
#define WINDOW_WINDOW_HPP_

#ifdef __APPLE__
    // Silence deprecation warnings
    #define GL_SILENCE_DEPRECATION
#endif

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.hpp"

class Window {
 private:
    // Main window
    GLFWwindow* _window;

    // Camera
    float _speed = 0.01;
    ptr_Camera _camera = nullptr;

    // Events
    void _handleKeys();

 protected:
    int _width, _height;

 public:
    /**
     * Window is a wrapper for GLFWwindow.
     * 
     * @param window_title: Title of the window.
     * @param width: Width of the window.
     * @param height: Height of the window.
     */
    Window(string window_title, int width, int height);
    ~Window();

    /**
     * Attaches the camera to the window.
     * @param camera: pointer to the camera.
     */
    void attachCamera(ptr_Camera camera);

    /**
     * @returns true if window is available.
     */
    virtual operator bool();

    /////////////
    // Getters //
    /////////////

    int getWidth() const;
    int getHeight() const;

    ptr_Camera getCamera() const;
};

#endif  // WINDOW_WINDOW_HPP_

