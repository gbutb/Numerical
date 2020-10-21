/**
 * laplace.hpp
 * -----------
 * 
 * Contains kernels for
 * solving laplace equation.
 */

#ifndef NUMERICAL_PRORGAM_LAPLACE_LAPLACE_HPP_
#define NUMERICAL_PRORGAM_LAPLACE_LAPLACE_HPP_

#include <math.h>
#include <opencv2/opencv.hpp>

#include "numerical/context.hpp"
#include "numerical/program/program.hpp"

class Laplace : public Program {
 private:
    int width, height;

    cl_kernel _laplace_kernel;

    bool parity = false;
    cl_mem _matrix_0;
    cl_mem _matrix_1;

 public:
    Laplace(Context& context, SolverOptions options);
    ~Laplace();

    /**
     * Takes input matrix, where boundary of the input
     * specifies the boundary condition and the interior
     * is set to output.
     * @param input Input data. CV_32FC1
     * @param output Location to width the output will be written.
     */
    void singleStep(cv::Mat& input, cv::Mat& output, int num_iterations = 10000);

    int getWidth() { return width; }
    int getHeight() { return height; }
};

#endif  // NUMERICAL_PRORGAM_LAPLACE_LAPLACE_HPP_
