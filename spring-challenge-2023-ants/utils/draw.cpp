#include "draw.h"

#define SIMULATED

#ifdef SIMULATED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat img(1000, 1000, CV_8UC4, cv::Scalar(255, 255, 255,0));

cv::Scalar floor_color(100, 100,0,0);
cv::Scalar crystal_color(0, 255, 255, 0);
cv::Scalar egg_color(0, 0, 128, 0);

cv::Scalar me_color(255, 0, 0, 0);
cv::Scalar opp_color(0, 0, 255, 0);


cv::Point2f pointy_hex_to_pixel(CubeCoord hex, double size){
    int x = sqrt(3)*size * (sqrt(3) * hex.q  +  sqrt(3)/2 * hex.r);
    int y = sqrt(3)*size * (3./2 * hex.r);
    return cv::Point2f(x, y);

}

void drawHexagon(Cell& c, cv::Scalar color, double s = 1){
    // using OPENCV
    // draw hexagon at coordinates
    CubeCoord coord = c.coord;
    //coord at 0,0 should draw at img center
    cv::Point2f center(img.cols/2, img.rows/2);
    cv::Size2f size(sqrt(3)*s, sqrt(3)*s);
    
    center += pointy_hex_to_pixel(coord, s);
    
    cv::Point2i vertices[6];
    for (int i = 0; i < 6; i++)
        vertices[i] = cv::Point2i(center.x + size.width * cos(i * 2 * CV_PI / 6 + CV_PI / 6),
            center.y + size.height * sin(i * 2 * CV_PI / 6 + CV_PI / 6));

    cv::fillConvexPoly(img, vertices, 6, color);

    for (int i = 0; i < 6; i++)
        cv::line(img, vertices[i], vertices[(i + 1) % 6], cv::Scalar(), 2);

    // add text with id on hexagon top part
    cv::putText(img, std::to_string(c.index), cv::Point2i(center.x - size.width/4, center.y - size.height/2), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 1);
}

void drawAnthill(CubeCoord coord, cv::Scalar color, double s = 1){
    // using OPENCV
    // draw hexagon at coordinates
    
    //coord at 0,0 should draw at img center
    cv::Point2f center(img.cols/2, img.rows/2);
    cv::Size2f size(sqrt(3)*s, sqrt(3)*s);
    
    center += pointy_hex_to_pixel(coord, s);
    
    cv::Point2i vertices[6];
    for (int i = 0; i < 6; i++)
        vertices[i] = cv::Point2i(center.x + size.width*.5 * cos(i * 2 * CV_PI / 6),
            center.y + size.height * .5 * sin(i * 2 * CV_PI / 6));

    cv::fillConvexPoly(img, vertices, 6, color);

    for (int i = 0; i < 6; i++)
        cv::line(img, vertices[i], vertices[(i + 1) % 6], cv::Scalar(), 1);
}

void drawCircle(CubeCoord coord, cv::Scalar color, double s = 1){

    cv::Point2f center(img.cols/2, img.rows/2);
    cv::Size2f size(sqrt(3)*s, sqrt(3)*s);
    
    center += pointy_hex_to_pixel(coord, s);
    
    // draw filled circle 
    cv::circle(img, center, size.width/6, color, -1);
}

void drawArena(Board& b){
    // using OPENCV

    // open window
    cv::namedWindow("Arena", cv::WINDOW_AUTOSIZE);

    // draw hexagons
    std::cout << b.distance(0,8) << std::endl;
    for(auto& c : b.cells){
        std::cout << c.index << " : " << c.coord.q << " " << c.coord.r << " " << c.coord.s << std::endl;
        drawHexagon(c, floor_color, 20);
        if(c.type == Cell::CRYSTAL){
            drawCircle(c.coord, crystal_color, 20);
        }else if(c.type == Cell::EGG){
            drawCircle(c.coord, egg_color, 20);
        }
    }

    for(int i : b.me->anthills){
        drawAnthill(b.cells[i].coord, me_color, 20);
    }
    for(int i : b.opp->anthills){
        drawAnthill(b.cells[i].coord, opp_color, 20);
    }
    // draw players
    // draw scores
    // draw round
    // draw time
    
    cv::imshow("Arena", img);
    cv::waitKey(0);
}
#else
void drawArena(Board& b){}
#endif