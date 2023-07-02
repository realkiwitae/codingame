#include "draw.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat img(600, 600, CV_8UC3, cv::Scalar(255, 255, 255));

void drawHexagon(CubeCoord coord, cv::Scalar color, double s = 1){
    // using OPENCV
    // draw hexagon at coordinates
    
    //coord at 0,0 should draw at img center
    cv::Point2f center(img.cols/2, img.rows/2);
    center.x += coord.x * 2 * s * cos(CV_PI/6);
    center.y += coord.y * 2 * s * sin(CV_PI/6);

    cv::Size2f size(s, s);
    cv::Point2f vertices[6];
    for (int i = 0; i < 6; i++)
        vertices[i] = cv::Point2f(center.x + size.width * cos(i * 2 * CV_PI / 6),
            center.y + size.height * sin(i * 2 * CV_PI / 6));
    for (int i = 0; i < 6; i++)
        cv::line(img, vertices[i], vertices[(i + 1) % 6], color, 2);
}

void drawArena(Board& b){
    // using OPENCV

    // open window
    cv::namedWindow("Arena", cv::WINDOW_AUTOSIZE);

    // draw hexagons

    for(auto& c : b.cells){
        drawHexagon(c.coord, cv::Scalar(0, 0, 0), 10);
    }


    // draw players
    // draw scores
    // draw round
    // draw time
    
    cv::imshow("Arena", img);
    cv::waitKey(0);
}