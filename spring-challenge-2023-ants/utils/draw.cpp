#include "draw.h"

#define SIMULATED

#ifdef SIMULATED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


cv::Mat img(1000, 1000, CV_8UC3, cv::Scalar(255, 255, 255));
cv::Mat background(1000, 1000, CV_8UC3, cv::Scalar(255, 255, 255));

cv::Scalar floor_color(100, 100,0);
cv::Scalar crystal_color(0, 255, 255);
cv::Scalar egg_color(0, 0, 128);

cv::Scalar me_color(255, 0, 0);
cv::Scalar opp_color(0, 0, 255);


cv::Point2f pointy_hex_to_pixel(CubeCoord hex, double size){
    int x = img.cols/2 + sqrt(3)*size * (sqrt(3) * hex.q  +  sqrt(3)/2 * hex.r);
    int y = img.rows/2 + sqrt(3)*size * (3./2 * hex.r);
    return cv::Point2f(x, y);
}

void drawHexagon(Cell& c, cv::Scalar color, double s = 1){
    // using OPENCV
    // draw hexagon at coordinates
    CubeCoord coord = c.coord;
    //coord at 0,0 should draw at img center
    cv::Size2f size(sqrt(3)*s, sqrt(3)*s);
    
    cv::Point2f center = pointy_hex_to_pixel(coord, s);
    
    cv::Point2i vertices[6];
    for (int i = 0; i < 6; i++)
        vertices[i] = cv::Point2i(center.x + size.width * cos(i * 2 * CV_PI / 6 + CV_PI / 6),
            center.y + size.height * sin(i * 2 * CV_PI / 6 + CV_PI / 6));

    cv::fillConvexPoly(img, vertices, 6, color);

    for (int i = 0; i < 6; i++)
        cv::line(img, vertices[i], vertices[(i + 1) % 6], cv::Scalar(), 2);

}

void drawAnthill(CubeCoord coord, cv::Scalar color, double s = 1){
    // using OPENCV
    // draw hexagon at coordinates
    
    //coord at 0,0 should draw at img center
    cv::Size2f size(sqrt(3)*s, sqrt(3)*s);
    
    cv::Point2f center = pointy_hex_to_pixel(coord, s);
    
    cv::Point2i vertices[6];
    for (int i = 0; i < 6; i++)
        vertices[i] = cv::Point2i(center.x + size.width*.3 * cos(i * 2 * CV_PI / 6),
            center.y + size.height * .3 * sin(i * 2 * CV_PI / 6));

    cv::fillConvexPoly(img, vertices, 6, color);

    for (int i = 0; i < 6; i++)
        cv::line(img, vertices[i], vertices[(i + 1) % 6], cv::Scalar(), 1);
}

void drawCircle(Cell& c, cv::Scalar color, double s = 1){

    double p = c.richness/100.;
    p = std::max(std::min(int((p+1/3.)*3)/3., 1.), 0.);

    cv::Size2f size(sqrt(3)*s*p , sqrt(3)*s*p);

    cv::Point2f center = pointy_hex_to_pixel(c.coord, s);    
    // draw filled circle 
    cv::circle(img, center, size.width*.3, color, -1);
}

void draw_init(Board& b){
        // draw hexagons
    for(auto& c : b.cells){
        drawHexagon(c, floor_color, 20);
    }

    for(int i : b.me->anthills){
        drawAnthill(b.cells[i].coord, me_color, 20);
    }
    for(int i : b.opp->anthills){
        drawAnthill(b.cells[i].coord, opp_color, 20);
    }

    // draw players
    cv::Mat me_png = cv::imread("./utils/image/me.jpg", cv::IMREAD_COLOR);
    cv::Mat opp_png = cv::imread("./utils/image/opp.png", cv::IMREAD_COLOR);
    cv::resize(me_png, me_png, cv::Size(100, 100));
    cv::resize(opp_png, opp_png, cv::Size(100, 100));
    me_png.copyTo(img(cv::Rect(0, 0, me_png.cols, me_png.rows)));
    opp_png.copyTo(img(cv::Rect(img.cols - opp_png.cols, 0, opp_png.cols, opp_png.rows)));

    // draw rectangle between players at top center of screen takes .5 of screen
    cv::rectangle(img, cv::Point2i(img.cols/2 - img.cols/4, 25), cv::Point2i(img.cols/2 + img.cols/4, 75), floor_color, -1);
    
    background = img.clone();
}

void drawArena(Board& b){
    // using OPENCV

    // open window
    cv::namedWindow("Arena", cv::WINDOW_AUTOSIZE);

    img = background.clone();
    // draw hexagons
    for(auto& c : b.cells){
        drawHexagon(c, floor_color, 20);
        if(c.type == Cell::CRYSTAL){
            drawCircle(c, crystal_color, 20);
        }else if(c.type == Cell::EGG){
            drawCircle(c, egg_color, 20);
        }
    }


    for(auto& c : b.cells){
        cv::Point2f center = pointy_hex_to_pixel(c.coord, 20);
      //  cv::putText(img, std::to_string(c.index), cv::Point2i(center.x-30, center.y+5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 1);
        if(c.ants[0] > 0){
            cv::putText(img, std::to_string(c.ants[0]), cv::Point2i(center.x-10, center.y+25), cv::FONT_HERSHEY_SIMPLEX, .5,me_color, 2);
        }
        if(c.ants[1]>0){
            cv::putText(img, std::to_string(c.ants[1]), cv::Point2i(center.x-10, center.y-15), cv::FONT_HERSHEY_SIMPLEX, .5,opp_color, 2);
        }

        if(c.beacons[0] > 0){
            cv::Point2i vertices[4];
            vertices[0] = cv::Point2i(center.x - 25, center.y - 10);
            vertices[1] = cv::Point2i(center.x - 10, center.y - 10);
            vertices[2] = cv::Point2i(center.x - 10, center.y + 10);
            vertices[3] = cv::Point2i(center.x - 25, center.y + 10);
            cv::fillConvexPoly(img, vertices, 4, me_color);
            cv::putText(img, std::to_string(c.beacons[0]), cv::Point2i(center.x-25, center.y+5), cv::FONT_HERSHEY_SIMPLEX, .3, cv::Scalar(0, 0, 0, 0), 1);
        }
        if(c.beacons[1] > 0){
            cv::Point2i vertices[4];
            vertices[0] = cv::Point2i(center.x + 10, center.y - 10);
            vertices[1] = cv::Point2i(center.x + 25, center.y - 10);
            vertices[2] = cv::Point2i(center.x + 25, center.y + 10);
            vertices[3] = cv::Point2i(center.x + 10, center.y + 10);
            cv::fillConvexPoly(img, vertices, 4, opp_color);
            cv::putText(img, std::to_string(c.beacons[1]), cv::Point2i(center.x+10, center.y+5), cv::FONT_HERSHEY_SIMPLEX, .3, cv::Scalar(0, 0, 0, 0), 1);
        }
    }


    // fill rectangle with player points: b.me->points / b.allCrystals
    cv::rectangle(img, cv::Point2i(img.cols/2 - img.cols/4, 25), cv::Point2i(img.cols/2 - img.cols/4 + (img.cols/2) * b.me->points / b.allCrystals, 75), me_color, -1);
    cv::rectangle(img, cv::Point2i(img.cols/2 + img.cols/4 - (img.cols/2) * b.opp->points / b.allCrystals, 25), cv::Point2i(img.cols/2 + img.cols/4, 75), opp_color, -1);

    // write points of each player in rectangle
    cv::putText(img, std::to_string(b.me->points), cv::Point2i(img.cols/2 - img.cols/4 + 10, 60), cv::FONT_HERSHEY_SIMPLEX, .8, cv::Scalar(0, 0, 0), 2);
    cv::putText(img, std::to_string(b.opp->points), cv::Point2i(img.cols/2 + img.cols/4 - 30, 60), cv::FONT_HERSHEY_SIMPLEX, .8, cv::Scalar(0, 0, 0), 2);
   
    // add line in middle of it to separate players, on top of the line write win condition, its equal to b.allCrystals/2
    cv::line(img, cv::Point2i(img.cols/2, 25), cv::Point2i(img.cols/2, 75), cv::Scalar(255, 255, 255), 2);
    cv::putText(img, std::to_string(b.allCrystals/2), cv::Point2i(img.cols/2 - 15, 20), cv::FONT_HERSHEY_SIMPLEX, .8, cv::Scalar(0, 0, 0), 2);

    // add b.round to bottom right corner
    cv::putText(img, std::to_string(b.round), cv::Point2i(img.cols - 50, img.rows - 10), cv::FONT_HERSHEY_SIMPLEX, .8, cv::Scalar(0, 0, 0), 2);


    cv::imshow("Arena", img);
    cv::waitKey(0);
}
#else
void drawArena(Board& b){}
#endif