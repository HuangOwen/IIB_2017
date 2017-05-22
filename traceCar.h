#ifndef TRACECAR_H_INCLUDED
#define TRACECAR_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

bool traceCar(cv::Mat,cv::Point&,cv::Point&);
double point_distance(cv::Point &,cv::Point &);


#endif // TRACECAR_H_INCLUDED
