/**
 * heat.hpp
 * --------
 * 
 * Contains kernels for
 * solving heat equation
 */

#ifndef NUMERICAL_PROGRAM_HEAT_HEAT_HPP_
#define NUMERICAL_PROGRAM_HEAT_HEAT_HPP_

#include <math.h>
#include <opencv2/opencv.hpp>
#include "numerical/program/program.hpp"


struct SolverOptions {
   double _time_step;
   double _space_step;

   int width;
   int height;
};

class Heat : public Program {
 private:
    int width, height;
    float ratio;

    cl_kernel _heat_kernel;
    cl_mem _input_heat_matrix;
    cl_mem _output_heat_matrix;

    cl_mem _width_buffer, _height_buffer;
    cl_mem _r_coeff_buffer;

 public:
    Heat(cl_context context, cl_device_id device, SolverOptions options); 
    ~Heat();

    /**
     * Takes input matrix and passes it through kernel.
     * @param input Input data.
     * @param output Location to which the output will be written.
     */
    void singleStep(cv::Mat& input, cv::Mat& output, cl_command_queue queue);
};

#endif  // NUMERICAL_PROGRAM_HEAT_HEAT_HPP_
