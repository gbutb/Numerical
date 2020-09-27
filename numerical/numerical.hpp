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

#include <functional>

// Load libs
#include <math.h>
#include <opencv2/opencv.hpp>


#include <window/window.hpp>
#include <window/shader/mapShader/mapShader.hpp>

#include "program/program.hpp"

class Numerical : public Window {
 private:
    shared_ptr<MapShader> map_shader;
    shared_ptr<Program> _program = nullptr;

    // State matrix
    double scale;
    cv::Mat state_mat;

    std::function<cv::Mat(int, int)> _initialCondition;

 public:
    Numerical(string window_title, int width, int height);
    ~Numerical();

    void registerProgram(shared_ptr<Program> program, std::function<cv::Mat(int, int)> initialCondition); 

    /**
     * @returns true if window is available.
     */
    operator bool();
};

#endif  // NUMERICAL_NUMERICAL_HPP_

