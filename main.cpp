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
    options._time_step = 0.1;
    Heat heat(numerical._context, numerical._device, options);

    cv::Mat input = cv::Mat::zeros(options.height, options.width, CV_32FC1);
    for (int i = -10; i < 10; ++i)
        for (int j = -10; j < 10; ++j)
            input.at<float>(128 + i, 128 + j) = 100.0;
    cv::Mat output = cv::Mat::zeros(options.height, options.width, CV_32FC1);
    for (int i = 0; i < 1000; ++i) {
        heat.singleStep(input, output, numerical._queue);
        output.copyTo(input);

        cv::Mat show;
        output.convertTo(show, CV_8U, 255);
        cv::applyColorMap(show, show, cv::COLORMAP_JET);
        cv::imshow("out", show);
        cv::waitKey(1);
        printf("%d/%d\n", i+1, 1000);
    }

    double minVal, maxVal;
    cv::minMaxLoc(output, &minVal, &maxVal, NULL, NULL);
    output = output / (100.0);

    printf("min %f and max %f\n", minVal, maxVal);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return EXIT_SUCCESS;
}
