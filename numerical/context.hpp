/**
 * context.hpp
 * -------------
 * 
 * Contains wrapper for OpenCL
 * kernels.
 */

#ifndef NUMERICAL_CONTEXT_HPP_
#define NUMERICAL_CONTEXT_HPP_

#include <OpenCL/opencl.h>

class Context {
 private:
    cl_context _context;
    cl_device_id _device;
    cl_command_queue _queue;

    /**
     * Initializes a device.
     */
    cl_device_id create_device();

 public:
    /**
     * @throw runtime_exception if there are any OpenCL errors.
     */
    Context();
    ~Context();

    Context(const Context& context) {
        _context = context._context;
        _device = context._device;
        _queue = context._queue;
    }

    Context & operator=(const Context&) = delete;

    /**
     * Getters
     */

    inline cl_context& getCLContext() { return _context; }
    inline cl_device_id& getDevice() { return _device; }
    inline cl_command_queue& getCommandQueue() { return _queue; }
};

#endif  // NUMERICAL_CONTEXT_HPP_
