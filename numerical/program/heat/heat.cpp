/**
 * heat.cpp
 * --------
 * 
 * Contains implementation of heat equation.
 */

#include "heat.hpp"

Heat::Heat(cl_context context, cl_device_id device, SolverOptions options) : Program::Program(context, device) {
    int err;

    // Parse options
    width = options.width;
    height = options.height;
    ratio = (float)options._time_step/pow(options._space_step, 2);

    // Initialize program
    _program = build_program(
        #include "heat.cl"
    );

    // Initialize kernel
    _heat_kernel = clCreateKernel(_program, "single_heat_step", &err);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel");

    // Allocate I/O buffers
    _input_heat_matrix = clCreateBuffer(
        _context, CL_MEM_READ_ONLY,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to transfer input matrix");

    _output_heat_matrix = clCreateBuffer(
        _context, CL_MEM_READ_WRITE,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize output buffer");
}

Heat::~Heat() {
    clReleaseMemObject(_input_heat_matrix);
    clReleaseMemObject(_output_heat_matrix);
    clReleaseKernel(_heat_kernel);
}

void Heat::singleStep(cv::Mat& input, cv::Mat& output, cl_command_queue queue) {
    int err;
    CV_Assert(input.type() == CV_32FC1);
    CV_Assert(output.type() == CV_32FC1);

    // Allocate memories
    err = clEnqueueWriteBuffer(
        queue, _input_heat_matrix,
        CL_TRUE, 0, width * height * sizeof(float),
        input.data, 0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to enqueue copy operation");

    // Initialize arguments
    err = clSetKernelArg(_heat_kernel, 0, sizeof(cl_mem), &_input_heat_matrix);
    err |= clSetKernelArg(_heat_kernel, 1, sizeof(cl_mem), &_output_heat_matrix);
    err |= clSetKernelArg(_heat_kernel, 2, sizeof(int), &width);
    err |= clSetKernelArg(_heat_kernel, 3, sizeof(int), &height);
    err |= clSetKernelArg(_heat_kernel, 4, sizeof(float), &ratio);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel arguments");

    // Enqueue the kernel
    size_t global_size = width * height;
    size_t local_size = 1;
    err = clEnqueueNDRangeKernel(queue, _heat_kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to enqueue kernel");

    // Read the output
    err = clEnqueueReadBuffer(
        queue, _output_heat_matrix, CL_TRUE, 0,
        width * height * sizeof(float), output.data,
        0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to read output");
}
