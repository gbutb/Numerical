/**
 * program.cpp
 * -----------
 * 
 * Implements program
 */

#include <string.h>
#include <stdexcept>

#include "program.hpp"

cl_program Program::build_program(const char* source) {
    int err;

    size_t program_size = strlen(source);
    cl_program program = clCreateProgramWithSource(
        getContext().getCLContext(), 1, (const char**)&source, &program_size, &err);
    if (err < 0)
        throw std::runtime_error("Unable to create the program");
    
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err < 0) {
        size_t log_size;
        clGetProgramBuildInfo(program, getContext().getDevice(), CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char* log = (char*)malloc(log_size + 1);
        log[log_size] = '\0';
        clGetProgramBuildInfo(program, getContext().getDevice(), CL_PROGRAM_BUILD_LOG, log_size + 1, log, NULL);
        printf("%s\n", log);
        free(log);
        throw std::runtime_error("Unable compile program");
    }

    return program;
}

Program::~Program() {
    clReleaseProgram(_program);
}
