/**
 * heat.cpp
 * --------
 * 
 * Contains implementation of heat equation.
 */

#include "heat.hpp"

Heat::Heat(cl_context context, cl_device_id device, SolverOptions options) : Program::Program(context, device) {
    _program = build_program(
        #include "heat.cl"
    );
}

Heat::~Heat() {
    clReleaseMemObject(_input_heat_matrix);
    clReleaseMemObject(_output_heat_matrix);
    clReleaseKernel(_heat_kernel);
}

void Heat::singleStep(cv::Mat& input, cv::Mat& output) {
}
