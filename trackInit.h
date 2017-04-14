#ifndef TRACKINIT_H_INCLUDED
#define TRACKINIT_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>


void help();  //the guider of initialization
int genTrack (cv::VideoCapture&); //to generate the track
cv::Mat changePerspective(cv::Mat);
void onMouse(int event, int x, int y, int flags, void* param); 





#endif // TRACKINIT_H_INCLUDED
