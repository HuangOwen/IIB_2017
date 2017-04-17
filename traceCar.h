#ifndef TRACECAR_H_INCLUDED
#define TRACECAR_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

bool traceCar(cv::Mat,cv::Point&,cv::Point&);
void drawCircle(cv::Mat&, const std::vector<cv::Vec3f>&);

#endif // TRACECAR_H_INCLUDED
