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

#include "numerical/program/heat/heat.hpp"

void printUsage() {
    printf("Usage: mainWindow -w [width] -h [height] -t [time_step] -x [space_step]\n");
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
    Numerical window("Test", 1280, 720);
    window.attachCamera(camera);

    // Configure solver
    Context context;
    SolverOptions options(width, height, time_step, space_step);

    shared_ptr<Heat> heat(new Heat(context, options));
    window.registerProgram(heat);

    while (window) {}
    return EXIT_SUCCESS;
}
