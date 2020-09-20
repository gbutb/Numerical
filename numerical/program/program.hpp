/**
 * program.hpp
 * -----------
 * 
 * Contains a general program abstract class.
 */

#ifndef NUMERICAL_PROGRAM_PROGRAM_HPP_
#define NUMERICAL_PROGRAM_PROGRAM_HPP_

#include <OpenCL/opencl.h>

#include "numerical/context.hpp"

class Program {
 private:
    Context _context;

    cl_program _program;

 protected:
    /**
     * Builds an OpenCL program given its source.
     * @param source Source of the OpenCL program.
     */
    cl_program build_program(const char* source);

 public:
    Program(Context& context) : _context(context) {}
    ~Program();

    /**
     * Getters
     */

    inline cl_program& getProgram() { return _program; }
    inline Context& getContext() { return _context; }
};

#endif  // NUMERICAL_PROGRAM_PROGRAM_HPP_
