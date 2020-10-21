/**
 * mainLaplace.cpp
 * ---------------
 * 
 * Contains code for laplace solver.
 */

#include <stdio.h>
#include <stdlib.h>

#include <fstream>
using std::ofstream;

#include <functional>

#include <numerical/numerical.hpp>
#include <numerical/program/laplace/laplace.hpp>

void printUsage() {
    printf("Usage: mainWindow -n [num_steps] -i [number of iterations]\n");
}

cv::Mat initializeBoundary(SolverOptions options) {
    cv::Mat map = cv::Mat::zeros(options.height, options.width, CV_32FC1);
    double step = options._space_step;
    for (int y = 0; y < options.height; ++y) {
        map.at<float>(y, 0) = sin(2 * M_PI * y * step) - y * step;
        map.at<float>(y, options.width - 1) = y * step + 1;
    }

    for (int x = 0; x < options.width; ++x) {
        map.at<float>(0, x) = 3 * sin(M_PI * x * step) + x * step;
        map.at<float>(options.height - 1, x) = 3 * x * step - 1;
    }
    return map;
}

void displayMat(const cv::Mat& mat) {
    double maxVal = -1;
    cv::minMaxLoc(mat, NULL, &maxVal, NULL, NULL);
    cv::Mat clone = mat.clone() / maxVal;
    clone.convertTo(clone, CV_8U, 255);
    cv::applyColorMap(clone, clone, cv::COLORMAP_JET);
    cv::imshow("Laplace", clone);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

int main(int argn, char** argv) {
    int width = -1, height = -1;
    int num = -1;
    int iterNum = 10000;

    char destination[500] = "\0";

    // Parse args
    int opt;
    while ((opt = getopt(argn, argv, "n:i:d:")) != -1) {
        switch (opt) {
            case 'n':
                num = atoi(optarg);
                break;
            case 'i':
                iterNum = atoi(optarg);
                break;
            case 'd':
                strcpy(destination, optarg);
                break;
        }
    }

    double space_step = 1.0 / num;
    width = num;
    height = num;

    if (width <= 0 || height <= 0 || space_step <= 0 || iterNum <= 0) {
        printUsage();
        return EXIT_FAILURE;
    }

    printf("-------------------\nx in [%.3f, %.3f]\ny in [%.3f, %.3f]\n-------------------\n",
        0.0, width * space_step, 0.0, height * space_step);


    Context context;
    SolverOptions options(width, height, space_step, space_step);
    cv::Mat map = initializeBoundary(options);

    Laplace laplace(context, options);
    laplace.singleStep(map, map, iterNum);

    displayMat(map);

    if (strlen(destination) == 0) return EXIT_SUCCESS;

    ofstream file(destination);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            file << x*space_step << " " << y*space_step << " " << map.at<float>(y, x) << "\n";
        }
        file << "\n";
    }
    file.close();

    return EXIT_SUCCESS;
}
