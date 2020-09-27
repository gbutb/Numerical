/**
 * numerical.cpp
 * -------------
 * 
 * Contains implementation of numerical.hpp
 */

#include "numerical.hpp"

/**
 * @return Initial condition u(0, -) s.t. it vanishes on boundary.
 */
cv::Mat initializeHeatmap(int width, int height) {
    cv::Mat input = cv::Mat::zeros(height, width, CV_32FC2);
    int downscale = 16;
    for (int y = -height/downscale; y < height/downscale - 1; ++y)
        for (int x = -width/downscale; x < width/downscale- 1; ++x)
            input.at<cv::Vec2f>(height/2 + y, width/2 + x) = cv::Vec2f(
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)),
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)));
    return input;
}

Numerical::Numerical(string window_title, int width, int height) :
        Window::Window(window_title, width, height) {
}

Numerical::~Numerical() {
    Window::~Window();
}

cv::Mat toSingle(const cv::Mat& input) {
    cv::Mat single;
    cv::extractChannel(input, single, input.channels() - 1);
    return single;
}

void Numerical::registerProgram(shared_ptr<Program> program) {
    // Initialize shader
    map_shader = std::make_shared<MapShader>(program->getWidth(), program->getHeight());
    state_mat = initializeHeatmap(program->getWidth(), program->getHeight());

    cv::minMaxLoc(state_mat, nullptr, &scale, nullptr, nullptr);


    map_shader->loadMatrix(toSingle(state_mat / scale));

    _program = program;
}

Numerical::operator bool() {
    bool result = Window::operator bool();

    if (map_shader != nullptr) {
        map_shader->setMatrix("model", glm::value_ptr(getCamera()->getModel()));
        map_shader->setMatrix("projection", glm::value_ptr(getCamera()->getProjection()));

        map_shader->render();
    }

    if (_program != nullptr)
        _program->singleStep(state_mat, state_mat);
    
    if (map_shader != nullptr)
        map_shader->loadMatrix(toSingle(state_mat / scale));

    return result;
}
