/**
 * numerical.hpp
 * -------------
 * 
 * A specialized window for programs.
 */

#ifndef NUMERICAL_NUMERICAL_HPP_
#define NUMERICAL_NUMERICAL_HPP_

#ifdef __APPLE__
    // Silence deprecation warnings
    #define GL_SILENCE_DEPRECATION
#endif

#include <window/window.hpp>
#include <window/shader/mapShader/mapShader.hpp>

class Numerical : public Window {
 private:
    shared_ptr<MapShader> map_shader;

 public:
    Numerical(string window_title, int width, int height);
    ~Numerical();

    /**
     * @returns true if window is available.
     */
    operator bool();
};

#endif  // NUMERICAL_NUMERICAL_HPP_

