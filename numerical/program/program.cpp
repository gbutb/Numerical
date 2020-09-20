/**
 * program.cpp
 * -----------
 * 
 * Implements program
 */

#include <string.h>
#include <stdexcept>

#include "program.hpp"

cl_program Program::build_program(const char* source) const {
    int err;

    size_t program_size = strlen(source);
    cl_program program = clCreateProgramWithSource(
        _context, 1, (const char**)&source, &program_size, &err);
    if (err < 0)
        throw std::runtime_error("Unable to create the program");
    
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
        throw std::runtime_error("Unable compile program");
    
    return program;
}
