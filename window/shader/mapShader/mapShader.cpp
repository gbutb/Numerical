/**
 * mapShader.cpp
 * -------------
 * 
 * Contains implementation of mapShader.hpp
 */

#include "mapShader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

MapShader::MapShader(int width, int height) : Shader::Shader(
        #include "vertex.vs"
        ,
        #include "fragment.fs"
    ) {
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    _width = width;
    _height = height;
}

MapShader::~MapShader() {
    Shader::~Shader();
    glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}

struct Point {
    float x, y;
    float value;
};

void MapShader::loadMatrix(const cv::Mat& matrix) {
    CV_Assert(matrix.type() == CV_32FC1);
    use();

    // Update uniforms
    setInt("width", _width);
    setInt("height", _height);

    Point* points = (Point*)malloc(sizeof(Point)*_width*_height);
    int scale = MAX(_width, _height);
    for (int y = 0; y < _height; ++y)
        for (int x = 0; x < _width; ++x)
            points[x + y*_width] = Point({
                .x = static_cast<float>(x) / scale,
                .y = static_cast<float>(y) / scale,
                .value = matrix.at<float>(y, x)});

    glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*_width*_height, points, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(points);
}

void MapShader::render() {
    use();

    // Focus to current buffers
    glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glEnableVertexAttribArray(0);
            for (int i = 0; i < _height; ++i) {
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void *) (i * _width * sizeof(Point)));
                glDrawArrays(GL_LINE_STRIP, 0, _width);
            }
            for (int i = 0; i < _width; ++i) {
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _width * sizeof(Point), (void*) (i * sizeof(Point)));
                glDrawArrays(GL_LINE_STRIP, 0, _height);
            }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
