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
    for (int y = -height/2; y < height/2 - 1; ++y)
        for (int x = -width/2; x < width/2 - 1; ++x)
            input.at<float>(height/2 + y, width/2 + x) =
                (1 - pow(2*x/(float)width, 2))*(1 - pow(2*y/(float)height, 2)) > 0.5 ? 100000 : 0;// *pow(sin((x + y)/100.0), 2);
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
    static_cast<cv::Mat>(mat / scale).convertTo(show, CV_8U, 255);
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

    Heat heat(context, options);
    cv::Mat heat_map = initializeHeatmap(width, height);

    // Start loop
    startVisualization(heat, heat_map, text);

    return EXIT_SUCCESS;
}
