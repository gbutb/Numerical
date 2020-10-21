/**
 * laplace.cpp
 * -----------
 * 
 * Contains implementation of Laplace equation.
 */

#include "laplace.hpp"

Laplace::Laplace(Context& context, SolverOptions options) : Program::Program(context) {
    int err;

    // Parse options
    width = options.width;
    height = options.height;

    getProgram() = build_program(
        #include "laplace.cl"
    );

    // Initialize kernel
    _laplace_kernel = clCreateKernel(getProgram(), "laplace", &err);
    if (err < 0)
        throw std::runtime_error("Unable to create kernel");

    // Allocate I/O buffers
    _matrix_0 = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_WRITE,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize input matrix");

    _matrix_1 = clCreateBuffer(
        getContext().getCLContext(), CL_MEM_READ_WRITE,
        width * height * sizeof(float), NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to initialize output matrix");
}

Laplace::~Laplace() {
    clReleaseMemObject(_matrix_0);
    clReleaseMemObject(_matrix_1);
    clReleaseKernel(_laplace_kernel);
}

void Laplace::singleStep(cv::Mat& input, cv::Mat& output, int num_iterations) {
    int err;
    CV_Assert(input.type() == CV_32FC1);
    CV_Assert(output.type() == CV_32FC1);

    // Prepare the queue
    cl_command_queue queue = getContext().getCommandQueue();

    // Allocate memories
    parity = false;
    err = clEnqueueWriteBuffer(
        queue, _matrix_0,
        CL_TRUE, 0, width * height * sizeof(float),
        input.data, 0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to enqueue copy operation");

    // Initialize arguments
    for (int i = 0; i < num_iterations; ++i) {
        err =
            clSetKernelArg(_laplace_kernel, 0, sizeof(cl_mem), !parity ? &_matrix_0 : &_matrix_1)  |
            clSetKernelArg(_laplace_kernel, 1, sizeof(cl_mem), !parity ? &_matrix_1 : &_matrix_0)  |
            clSetKernelArg(_laplace_kernel, 2, sizeof(int), &width)                                |
            clSetKernelArg(_laplace_kernel, 3, sizeof(int), &height);
        if (err < 0)
            throw std::runtime_error("Unable to create kernel arguments");

        // Enqueue the kernel
        size_t global_size[] = {static_cast<size_t>(width), static_cast<size_t>(height)};
        size_t local_size[] = {1, 1};
        err = clEnqueueNDRangeKernel(queue, _laplace_kernel, 2, NULL, global_size, local_size, 0, NULL, NULL);
        if (err < 0)
            throw std::runtime_error("Unable to enqueue kernel");
        parity = !parity;
    }

    // Read the output
    err = clEnqueueReadBuffer(
        queue, !parity ? _matrix_1 : _matrix_0, CL_TRUE, 0,
        width * height * sizeof(float), output.data,
        0, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable to read output");
}
