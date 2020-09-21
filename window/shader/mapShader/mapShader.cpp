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

    indices = new int[width*height];
    for (int i = 0; i < width*height; ++i)
        indices[i] = i;
}

MapShader::~MapShader() {
    Shader::~Shader();
    glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);

    delete indices;
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

    Point points[_height][_width];
    for (int y = 0; y < _height; ++y)
        for (int x = 0; x < _width; ++x)
            points[y][x] = Point({
                .x = static_cast<float>(x) / _width,
                .y = static_cast<float>(y) / _height,
                .value = matrix.at<float>(y, x)});

    glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MapShader::render() {
    use();

    // Focus to current buffers
    glBindVertexArray(_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glEnableVertexAttribArray(0);
            for (int i = 0; i < _height; ++i) {
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, _width * sizeof(Point), (void *)(i * sizeof(Point)));
                glDrawArrays(GL_LINE_STRIP, 0, _height);
            }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
