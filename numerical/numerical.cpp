/**
 * numerical.cpp
 * -------------
 * 
 * Contains implementation of numerical.hpp
 */

#include "numerical.hpp"

Numerical::Numerical(string window_title, int width, int height) :
        Window::Window(window_title, width, height) {
    map_shader = std::make_shared<MapShader>(100, 100);
    cv::Mat mat = cv::Mat::zeros(100, 100, CV_32FC1);
    map_shader->loadMatrix(mat);
}

Numerical::~Numerical() {
    Window::~Window();
}

Numerical::operator bool() {
    bool result = Window::operator bool();

    map_shader->setMatrix("model", glm::value_ptr(getCamera()->getModel()));
    map_shader->setMatrix("projection", glm::value_ptr(getCamera()->getProjection()));

    map_shader->render();

    return result;
}
