/**
 * mapShader.hpp
 * -------------
 * 
 * Map shader is a shader for
 * rendering OpenCV matrices.
 */

#ifndef WINDOW_SHADER_MAPSHADER_MAPSHADER_HPP_
#define WINDOW_SHADER_MAPSHADER_MAPSHADER_HPP_

#include <math.h>
#include <opencv2/opencv.hpp>
#include "window/shader/shader.hpp"

class MapShader : public Shader {
 private:
    // Buffers
    unsigned int _VBO, _VAO;

    int _width, _height;

 public:
    MapShader(int width, int height);
    ~MapShader();

    /**
     * Loads matrix into buffers.
     */
    void loadMatrix(const cv::Mat& matrix);


    void render();
};

#endif  // WINDOW_SHADER_MAPSHADER_HPP_

