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

class Heat : public Program {
 private:
    double _time_step = -1;
    double _space_step = -1;
 public:
    Heat(cl_context context, cl_device_id device) : Program::Program(context, device) {}

    /**
     * Takes input matrix and passes it through kernel.
     * @param input Input data.
     * @param output Location to which the output will be written.
     */
    void singleStep(cv::Mat& input, cv::Mat& output);
};

#endif  // NUMERICAL_PROGRAM_HEAT_HEAT_HPP_
