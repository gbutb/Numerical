/**
 * wave.cpp
 * --------
 * 
 * Contains implementation of the wave equation.
 */

#include "wave.hpp"

Wave::Wave(Context& context, SolverOptions options) : Program::Program(context) {
    int err;

    // Parse options
    width = options.width;
    height = options.height;
    ratio = (float)options._time_step/options._space_step;

    if (pow(ratio,2) > 1/2.0)
        printf("Ratio is %f > 1/sqrt(2). The solution might not converge!\n", ratio);

    getProgram() = build_program(
        #include "wave.cl"
    );

    // Initialize kernel
    _wave_kernel = clCreateKernel(getProgram(), "single_wave_step", &err);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel");

    // Allocate I/O buffers
    _input_wave_matrix = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_ONLY,
        width * height * 2 * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize input matrix");

    _output_wave_matrix = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_WRITE,
        width * height * 2 * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize output buffer");
}

Wave::~Wave() {
    clReleaseMemObject(_input_wave_matrix);
    clReleaseMemObject(_output_wave_matrix);
    clReleaseKernel(_wave_kernel);
}

void Wave::singleStep(cv::Mat& input, cv::Mat& output) {
    int err;
    CV_Assert(input.type() == CV_32FC2);
    CV_Assert(output.type() == CV_32FC2);

    // Prepare the queue
    cl_command_queue queue = getContext().getCommandQueue();

    // Allocate memories
    err = clEnqueueWriteBuffer(
        queue, _input_wave_matrix,
        CL_TRUE, 0, width * height * 2 * sizeof(float),
        input.data, 0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to enqueue copy operation");

    // Initialize arguments
    err = clSetKernelArg(_wave_kernel, 0, sizeof(cl_mem), &_input_wave_matrix);
    err |= clSetKernelArg(_wave_kernel, 1, sizeof(cl_mem), &_output_wave_matrix);
    err |= clSetKernelArg(_wave_kernel, 2, sizeof(int), &width);
    err |= clSetKernelArg(_wave_kernel, 3, sizeof(int), &height);
    err |= clSetKernelArg(_wave_kernel, 4, sizeof(float), &ratio);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel arguments");

    // Enqueue the kernel
    size_t global_size[] = {static_cast<size_t>(width), static_cast<size_t>(height)};
    size_t local_size[] = {16, 16};
    err = clEnqueueNDRangeKernel(queue, _wave_kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to enqueue kernel");

    // Read the output
    err = clEnqueueReadBuffer(
        queue, _output_wave_matrix, CL_TRUE, 0,
        width * height * 2 * sizeof(float), output.data,
        0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to read output");
}
