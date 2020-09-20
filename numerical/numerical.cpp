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
