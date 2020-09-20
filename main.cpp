/**
 * main.cpp
 * --------
 */

// Load standard stuff
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Load libs
#include <opencv2/opencv.hpp>

#include <numerical/numerical.hpp>
#include <numerical/program/heat/heat.hpp>

int main(void) {
    Numerical numerical;

    SolverOptions options;
    options.width = 256;
    options.height = 256;
    options._space_step = 1.0;
    options._time_step = 10.0;
    Heat heat(numerical._context, numerical._device, options);

    cv::Mat input = cv::Mat::ones(options.height, options.width, CV_32FC1);
    cv::Mat output = cv::Mat::zeros(options.height, options.width, CV_32FC1);
    heat.singleStep(input, output, numerical._queue);
    return EXIT_SUCCESS;
}
