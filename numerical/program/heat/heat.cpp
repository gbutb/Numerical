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

    // Allocate buffers
    _r_coeff_buffer = clCreateBuffer(
        _context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(float), &ratio, &err);
    if (err < 0)
        throw std::runtime_error("Unable to allocate r coefficient buffer");
    _width_buffer = clCreateBuffer(
        _context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(int), &width, &err);
    if (err < 0)
        throw std::runtime_error("Unable to allocate width buffer");
    _height_buffer = clCreateBuffer(
        _context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(int), &height, &err);
    if (err < 0)
        throw std::runtime_error("Unable to allocate height buffer");
    
}

Heat::~Heat() {
    clReleaseMemObject(_input_heat_matrix);
    clReleaseMemObject(_output_heat_matrix);
    clReleaseMemObject(_width_buffer);
    clReleaseMemObject(_height_buffer);
    clReleaseMemObject(_r_coeff_buffer);
    clReleaseKernel(_heat_kernel);
}

void Heat::singleStep(cv::Mat& input, cv::Mat& output, cl_command_queue queue) {
    int err;
    CV_Assert(input.type() == CV_32FC1);
    CV_Assert(output.type() == CV_32FC1);

    // Allocate memories
    _input_heat_matrix = clCreateBuffer(
        _context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        width * height * sizeof(float), input.data, &err);
    if (err < 0)
        throw std::runtime_error("Unable to transfer input matrix");

    _output_heat_matrix = clCreateBuffer(
        _context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
        width * height * sizeof(float), output.data, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize output buffer");

    // Initialize arguments
    err = clSetKernelArg(_heat_kernel, 0, sizeof(cl_mem), &_input_heat_matrix);
    err |= clSetKernelArg(_heat_kernel, 1, sizeof(cl_mem), &_output_heat_matrix);
    err |= clSetKernelArg(_heat_kernel, 2, sizeof(cl_mem), &_width_buffer);
    err |= clSetKernelArg(_heat_kernel, 3, sizeof(cl_mem), &_height_buffer);
    err |= clSetKernelArg(_heat_kernel, 4, sizeof(cl_mem), &_r_coeff_buffer);
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
