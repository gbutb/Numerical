/**
 * numerical.cpp
 * -------------
 * 
 * Contains implementation of numerical.hpp
 */

#include "numerical.hpp"


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

void Numerical::registerProgram(shared_ptr<Program> program, std::function<cv::Mat(int, int)> initialCondition) {
    this->_initialCondition = initialCondition;

    // Initialize shader
    map_shader = std::make_shared<MapShader>(program->getWidth(), program->getHeight());
    state_mat = initialCondition(program->getWidth(), program->getHeight());

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
