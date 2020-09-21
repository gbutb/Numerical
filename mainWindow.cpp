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

int main(void) {
    ptr_Camera camera(new Camera(1280, 720));
    Numerical window("Test", 1280, 720);
    window.attachCamera(camera);

    // Configure solver
    Context context;
    SolverOptions options(500, 500, 0.001, 1.0);

    shared_ptr<Heat> heat(new Heat(context, options));
    window.registerProgram(heat);

    while (window) {}
    return EXIT_SUCCESS;
}
