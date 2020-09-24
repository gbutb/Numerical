/**
 * heat.cpp
 * --------
 * 
 * Contains implementation of heat equation.
 */

#include "heat.hpp"

Heat::Heat(Context& context, SolverOptions options) : Program::Program(context) {
    int err;

    // Parse options
    width = options.width;
    height = options.height;
    ratio = (float)options._time_step/pow(options._space_step, 2);

    if (ratio > 1/4.0)
        printf("Ratio is %f > 1/4.0. The solution might not converge!\n", ratio);

    // Initialize program
    getProgram() = build_program(
        #include "heat.cl"
    );

    // Initialize kernel
    _heat_kernel = clCreateKernel(getProgram(), "single_heat_step", &err);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel");

    // Allocate I/O buffers
    _input_heat_matrix = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_ONLY,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to transfer input matrix");

    _output_heat_matrix = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_WRITE,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize output buffer");
}

Heat::~Heat() {
    clReleaseMemObject(_input_heat_matrix);
    clReleaseMemObject(_output_heat_matrix);
    clReleaseKernel(_heat_kernel);
}

void Heat::singleStep(cv::Mat& input, cv::Mat& output) {
    int err;
    CV_Assert(input.type() == CV_32FC1);
    CV_Assert(output.type() == CV_32FC1);

    // Prepare the queue
    cl_command_queue queue = getContext().getCommandQueue();

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
    size_t global_size[] = {static_cast<size_t>(width), static_cast<size_t>(height)};
    size_t local_size[] = {16, 16};
    err = clEnqueueNDRangeKernel(queue, _heat_kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
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
