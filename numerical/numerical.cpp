/**
 * numerical.cpp
 * -------------
 * 
 * Contains implementation of Numerical.
 */

#include <stdexcept>
#include "numerical.hpp"
#include "program/heat/heat.hpp"


Numerical::Numerical() {
    int err;

    _device = create_device();
    _context = clCreateContext(NULL, 1, &_device, NULL, NULL, &err);
    if (err < 0)
        throw std::runtime_error("Unable to create a context");
    _queue = clCreateCommandQueue(_context, _device, 0, &err);
    if (err < 0)
        throw std::runtime_error("Unable to create a command queue");
}

Numerical::~Numerical() {
    clReleaseCommandQueue(_queue);
    clReleaseContext(_context);
}

cl_device_id Numerical::create_device() {
    cl_platform_id platform;
    cl_device_id dev;
    int err;

    // Get current platform
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err < 0)
        throw std::runtime_error("Couldn't Identify a platform");

    // Get device
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if (err == CL_DEVICE_NOT_FOUND) {
        printf("GPU not found, trying CPU instead\n");
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    if (err < 0)
        throw std::runtime_error("Coundn't access any devices");

    return dev;
}
