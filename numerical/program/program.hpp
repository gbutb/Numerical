/**
 * program.hpp
 * -----------
 * 
 * Contains a general program abstract class.
 */

#ifndef NUMERICAL_PROGRAM_PROGRAM_HPP_
#define NUMERICAL_PROGRAM_PROGRAM_HPP_

#include <OpenCL/opencl.h>

class Program {
 protected:
    cl_context _context;
    cl_device_id _device;

    cl_program _program;

    /**
     * Builds an OpenCL program given its source.
     * @param source Source of the OpenCL program.
     */
    cl_program build_program(const char* source) const;

 public:
    Program(cl_context context, cl_device_id device) : _context(context), _device(device) {}
    ~Program();
};

#endif  // NUMERICAL_PROGRAM_PROGRAM_HPP_
