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
#include <numerical/program/wave/wave.hpp>

/**
 * @return Initial condition u(0, -) s.t. it vanishes on boundary.
 */
cv::Mat initializeHeatmap(int width, int height) {
    cv::Mat input = cv::Mat::zeros(height, width, CV_32FC2);
    int downscale = 16;
    for (int y = -height/downscale; y < height/downscale - 1; ++y)
        for (int x = -width/downscale; x < width/downscale- 1; ++x)
            input.at<cv::Vec2f>(height/2 + y, width/2 + x) = cv::Vec2f(
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)),
                10*cos(3.1415*x*downscale/(2*width))*cos(3.1415*y*downscale/(2*height)));
    return input;
}

void printUsage() {
    printf("Usage: main -w [width] -h [height] -t [time_step] -x [space_step]\n");
}

static void onMouse(int event, int x, int y, int f, void* data){
    cv::Mat* mat = (cv::Mat*)data;

    // Initialize canvas
    cv::Mat canvas = cv::Mat::zeros(cv::Size(500, 100), CV_8U);

    // Draw the text onto canvas
    char heatText[500];
    sprintf(heatText, "Heat: %f", mat->at<float>(y, x));
    cv::putText(
        canvas, heatText, cv::Point(0, 50),
        cv::FONT_HERSHEY_DUPLEX,
        1.0, CV_RGB(255, 255, 255), 2);
    
    // Display canvas
    cv::imshow("value", canvas);
}

bool displayMat(const char* win_name, cv::Mat& mat, double scale, bool text) {
    cv::Mat show;
    cv::extractChannel(mat / scale, show, mat.channels() - 1);
    show.convertTo(show, CV_8U, 255);
    cv::applyColorMap(show, show, cv::COLORMAP_JET);
    if (text)
        cv::setMouseCallback(win_name, onMouse, &mat);
    cv::imshow(win_name, show);
    if (cv::waitKey(1) % 256 == 'q')
        return true;
    return false;
}

void startVisualization(Program& program, cv::Mat input, bool text = false) {
    cv::Mat current = input.clone();
    double maxVal = -1;
    cv::minMaxLoc(current, NULL, &maxVal, NULL, NULL);
    for (int i = 0;; ++i) {
        program.singleStep(input, current);
        current.copyTo(input);

        printf("%d\n", i);
        if (displayMat("Heatmap", current, maxVal, text))
            break;
    }

    cv::destroyAllWindows();
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


    // Configure solver and context
    Context context;
    SolverOptions options(width, height, time_step, space_step);

    Wave wave(context, options);
    cv::Mat wave_map = initializeHeatmap(width, height);

    // Start loop
    startVisualization(wave, wave_map, text);

    return EXIT_SUCCESS;
}
