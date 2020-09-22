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
    cv::Mat input = cv::Mat::zeros(height, width, CV_32FC1);
    for (int y = -height/2; y < height/2 - 1; ++y)
        for (int x = -width/2; x < width/2 - 1; ++x)
            input.at<float>(height/2 + y, width/2 + x) =
                (1 - pow(2*x/(float)width, 2))*(1 - pow(2*y/(float)height, 2)) > .5 ? 1000000 : 0; //*pow(sin(x/100.0)*sin(y/100.0), 2);
    return input;
}

Numerical::Numerical(string window_title, int width, int height) :
        Window::Window(window_title, width, height) {
}

Numerical::~Numerical() {
    Window::~Window();
}

void Numerical::registerProgram(shared_ptr<Program> program) {
    // Initialize shader
    map_shader = std::make_shared<MapShader>(program->getWidth(), program->getHeight());
    state_mat = initializeHeatmap(program->getWidth(), program->getHeight());

    cv::minMaxLoc(state_mat, nullptr, &scale, nullptr, nullptr);
    map_shader->loadMatrix(state_mat / scale);

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
        map_shader->loadMatrix(state_mat / scale);

    return result;
}
