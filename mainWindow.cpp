/**
 * mainWindow.cpp
 * --------------
 * 
 * Contains code for running GL window
 */

#include <stdio.h>
#include <stdlib.h>

#include "window/window.hpp"
#include "window/camera.hpp"

int main(void) {
    ptr_Camera camera(new Camera(1280, 720));
    Window window("Test", 1280, 720);
    window.attachCamera(camera);
    while (window) {}
    return EXIT_SUCCESS;
}
