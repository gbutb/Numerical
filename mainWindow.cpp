/**
 * mainWindow.cpp
 * --------------
 * 
 * Contains code for running GL window
 */

#include <stdio.h>
#include <stdlib.h>

#include "window/window.hpp"


int main(void) {
    Window window("Test", 1280, 720);
    while (window) {}
    return EXIT_SUCCESS;
}
