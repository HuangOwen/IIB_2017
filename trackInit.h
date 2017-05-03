#ifndef TRACKINIT_H_INCLUDED
#define TRACKINIT_H_INCLUDED

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>



void help();  //the guider of initialization
int genTrack (cv::VideoCapture&); //to generate the track
cv::Mat changePerspective(cv::Mat);
void onMouse(int event, int x, int y, int flags, void* param); 
cv::Mat binarize(cv::Mat);
cv::Mat sharpen(cv::Mat);
void drawHoughLines(cv::Mat&);
void genTurningPos(cv::Mat&);
bool isSameLine(cv::Vec4i,cv::Vec4i);
cv::Point getIntersec(cv::Vec4i,cv::Vec4i);
double xyPointDist(double,double,double,double);
void drawCircle(cv::Mat&, const std::vector<cv::Vec3f>&);





#endif // TRACKINIT_H_INCLUDED
