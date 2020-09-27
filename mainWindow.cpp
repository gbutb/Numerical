/**
 * mainWindow.cpp
 * --------------
 * 
 * Contains code for running GL window
 */

#include <stdio.h>
#include <stdlib.h>

#include "numerical/numerical.hpp"
#include "window/camera.hpp"

#include <numerical/program/wave/wave.hpp>

void printUsage() {
    printf("Usage: mainWindow -w [width] -h [height] -t [time_step] -x [space_step]\n");
}

/**
 * @return Initial condition u(0, -) s.t. it vanishes on boundary.
 */
cv::Mat initializeWave(int width, int height) {
    cv::Mat input = cv::Mat::zeros(height, width, CV_32FC2);
    int downscale = 16;
    for (int y = -height/downscale; y < height/downscale - 1; ++y)
        for (int x = -width/downscale; x < width/downscale- 1; ++x)
            input.at<cv::Vec2f>(height/2 + y, width/2 + width/8 + x) = cv::Vec2f(
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)),
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)));
    return input;
}

int main(int argn, char** argv) {
    int width = -1, height = -1;
    float time_step = -1, space_step = -1;
    bool text = false;

    // Parse args
    int opt;
    while ((opt = getopt(argn, argv, "w:h:t:x:v")) != -1) {
        switch (opt) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 't':
                time_step = atof(optarg);
                break;
            case 'x':
                space_step = atof(optarg);
                break;
            case 'v':
                text = true;
                break;
        }
    }

    if (width <= 0 || height <= 0 || time_step <= 0 || space_step <= 0) {
        printUsage();
        return EXIT_FAILURE;
    }

    ptr_Camera camera(new Camera(1280, 720));
    Numerical window("Wave 3D", 1280, 720);
    window.attachCamera(camera);

    // Configure solver
    Context context;
    SolverOptions options(width, height, time_step, space_step);

    shared_ptr<Wave> wave(new Wave(context, options));
    window.registerProgram(wave, initializeWave);

    while (window) {}
    return EXIT_SUCCESS;
}
