#ifndef CONTROLCAR_H_INCLUDED
#define CONTROLCAR_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

void go2Target(cv::Point,cv::Point,std::queue<cv::Point,std::list<cv::Point> > );

#endif // CONTROLCAR_H_INCLUDED
