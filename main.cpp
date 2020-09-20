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

#include <numerical/context.hpp>
#include <numerical/program/heat/heat.hpp>

/**
 * @return Initial condition u(0, -) s.t. it vanishes on boundary.
 */
cv::Mat initializeHeatmap(int width, int height) {
    cv::Mat input = cv::Mat::zeros(height, width, CV_32FC1);
    for (int y = -10; y < 10; ++y)
        for (int x = -10; x < 10; ++x)
            input.at<float>(128 + 2*y, 128 + x) = (x*sin(x/5.0) + y*sin(y/5.0));
    return input;
}

void printUsage() {
    printf("Usage: main -w [width] -h [height] -t [time_step] -x [space_step]\n");
}

bool displayMat(const char* win_name, const cv::Mat& mat) {
    cv::Mat show;
    mat.convertTo(show, CV_8U, 255);
    cv::applyColorMap(show, show, cv::COLORMAP_JET);
    cv::imshow(win_name, show);
    if (cv::waitKey(1) % 256 == 'q')
        return true;
    return false;
}

void startVisualization(Heat& heat, cv::Mat input) {
    cv::Mat current = input.clone();
    double maxVal = -1;
    cv::minMaxLoc(current, NULL, &maxVal, NULL, NULL);
    for (int i = 0;; ++i) {
        heat.singleStep(input, current);
        current.copyTo(input);

        printf("%d\n", i);
        if (displayMat("Window", current / maxVal))
            break;
    }

    cv::destroyAllWindows();
}

int main(int argn, char** argv) {
    int width = -1, height = -1;
    float time_step = -1, space_step = -1;

    // Parse args
    int opt;
    while ((opt = getopt(argn, argv, "w:h:t:x:")) != -1) {
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
        }
    }

    if (width <= 0 || height <= 0 || time_step <= 0 || space_step <= 0) {
        printUsage();
        return EXIT_FAILURE;
    }


    // Configure solver and context
    Context context;
    SolverOptions options(width, height, time_step, space_step);

    Heat heat(context, options);
    cv::Mat heat_map = initializeHeatmap(width, height);

    // Start loop
    startVisualization(heat, heat_map);

    return EXIT_SUCCESS;
}
