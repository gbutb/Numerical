/**
 * numerical.hpp
 * -------------
 * 
 * Contains wrapper for OpenCL
 * kernerls.
 */

#ifndef NUMERICAL_NUMERICAL_HPP_
#define NUMERICAL_NUMERICAL_HPP_

#include <OpenCL/opencl.h>

class Numerical {
 public:
    cl_device_id _device;
    cl_context _context;
    cl_command_queue _queue;

    /**
     * Initializes a device.
     */
    cl_device_id create_device();

 public:
    /**
     * @throw runtime_exception if there are any OpenCL errors.
     */
    Numerical();
    ~Numerical();
};

#endif  // NUMERICAL_NUMERICAL_HPP_
