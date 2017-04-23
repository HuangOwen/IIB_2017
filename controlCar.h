#ifndef CONTROLCAR_H_INCLUDED
#define CONTROLCAR_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

void go2Target(cv::Point,cv::Point,std::deque<cv::Point>& );
double point_distance(cv::Point &,cv::Point &);

#endif // CONTROLCAR_H_INCLUDED
