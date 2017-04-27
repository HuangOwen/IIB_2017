#include <iostream>
#include <cmath>
#include "trackInit.h"
#include "traceCar.h"

using namespace std;
using namespace cv;


//the HSV range
const Scalar hsvRedL1( 0,  70,  40);
const Scalar hsvRedH1(10, 200, 220);
const Scalar hsvRedL2(170, 70, 50);
const Scalar hsvRedH2(180, 200, 220);
const Scalar hsvGreenL(41,  40,  40);
const Scalar hsvGreenH(90, 255, 220);


bool traceCar(Mat src, Point& frontPos, Point& rearPos){
	Mat maskRed1(src.size().height,src.size().width,CV_8U);
	Mat maskRed2(src.size().height,src.size().width,CV_8U);
	Mat maskRed(src.size().height,src.size().width,CV_8U);
	Mat maskGreen(src.size().height,src.size().width,CV_8U);
	vector<Vec3f> greenCircles;
	vector<Vec3f> redCircles;
	
	inRange(src, hsvRedL1, hsvRedH1, maskRed1);
	inRange(src, hsvRedL2, hsvRedH2, maskRed2);
	inRange(src, hsvGreenL, hsvGreenH, maskGreen);
	maskRed = maskRed1 | maskRed2;


	HoughCircles(maskRed, redCircles, CV_HOUGH_GRADIENT, 2, 50, 200, 20);
	HoughCircles(maskGreen, greenCircles, CV_HOUGH_GRADIENT, 2, 50, 200, 20);

	imshow("maskRed",maskRed);
	imshow("maskGreen",maskGreen);

	
	if(redCircles.size()>0 && greenCircles.size()>0){
		frontPos = Point(redCircles[0][0],redCircles[0][1]);
		rearPos = Point(greenCircles[0][0],greenCircles[0][1]);
	 	cout<<"Car position detected!"<<endl;
		return true;
	}
	else{
		cout<<"Can not detect car position."<<endl;
		return false;
	}
	
}


void drawCircle(Mat &input, const vector<Vec3f> &circles){
    for(int i=0; i<circles.size(); i++){
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(input, center, radius, Scalar(255,0,0), 3, 8, 0 );
    }
}