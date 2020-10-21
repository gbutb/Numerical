/**
 * wave.hpp
 * --------
 * 
 * Contains kernels for
 * solving wave equation.
 */

#ifndef NUMERICAL_PROGRAM_WAVE_WAVE_HPP_
#define NUMERICAL_PROGRAM_WAVE_WAVE_HPP_

#include <math.h>
#include <opencv2/opencv.hpp>

#include "numerical/context.hpp"
#include "numerical/program/program.hpp"

class Wave : public Program {
 private:
    int width, height;
    float ratio;

    cl_kernel _wave_kernel;
    cl_mem _input_wave_matrix;
    cl_mem _output_wave_matrix;

 public:
    Wave(Context& context, SolverOptions options);
    ~Wave();

    /**
     * Takes input matrix and passes it through kernel.
     * @param input two-channel input data. CV_32FC2.
     * @param output Location to which the output will be written to. CV_32FC2.
     */
    void singleStep(cv::Mat& input, cv::Mat& output, int num_iterations = 10);

    int getWidth() { return width; }
    int getHeight() { return height; }
};

#endif  // NUMERICAL_PROGRAM_WAVE_WAVE_HPP_

