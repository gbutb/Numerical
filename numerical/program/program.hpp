/**
 * program.hpp
 * -----------
 * 
 * Contains a general program abstract class.
 */

#ifndef NUMERICAL_PROGRAM_PROGRAM_HPP_
#define NUMERICAL_PROGRAM_PROGRAM_HPP_

#include <math.h>
#include <opencv2/opencv.hpp>

#include <OpenCL/opencl.h>

#include "numerical/context.hpp"

struct SolverOptions {
   double _time_step;
   double _space_step;

   int width;
   int height;

   SolverOptions(int width, int height, float time_step, float space_step) {
      this->width = width;
      this->height = height;
      this->_time_step = time_step;
      this->_space_step = space_step;
   }
};


class Program {
 private:
    Context& _context;

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
     * Takes input matrix and passes it through kernel.
     * @param input Input data.
     * @param output Location to which the output will be written.
     */
    virtual void singleStep(cv::Mat& input, cv::Mat& output, int num_iterations = 10) {};

    /**
     * Getters
     */

    inline cl_program& getProgram() { return _program; }
    inline Context& getContext() { return _context; }

    virtual int getWidth() { return 0; }
    virtual int getHeight() { return 0; }
};

#endif  // NUMERICAL_PROGRAM_PROGRAM_HPP_
