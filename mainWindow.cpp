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

int main(void) {
    ptr_Camera camera(new Camera(1280, 720));
    Numerical window("Test", 1280, 720);
    window.attachCamera(camera);
    while (window) {}
    return EXIT_SUCCESS;
}
