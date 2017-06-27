#include <iostream>
#include <cmath>
#include "trackInit.h"
#include "traceCar.h"

#define MIN_RADIUS 10
#define MAX_RADIUS 30
#define CAR_LENGTH 70 //the minimum distance from rear pos to front pos

using namespace std;
using namespace cv;



//the HSV range  H:hue; S:saturate; V:value
const Scalar hsvRedL1( 0,  70,  40);
const Scalar hsvRedH1(10, 200, 220);
const Scalar hsvRedL2(170, 70, 50);
const Scalar hsvRedH2(180, 200, 220);
const Scalar hsvGreenL(30,  40,  30);
const Scalar hsvGreenH(80, 200, 255);  
 

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

	// red is accurate
	HoughCircles(maskRed, redCircles, CV_HOUGH_GRADIENT, 2, 50, 200, 20,MIN_RADIUS,MAX_RADIUS);
	// green is dummy
	HoughCircles(maskGreen, greenCircles, CV_HOUGH_GRADIENT, 2, 50, 200, 20,MIN_RADIUS,MAX_RADIUS);

	imshow("maskRed",maskRed);
	imshow("maskGreen",maskGreen);

	
	// if(redCircles.size()>0 && greenCircles.size()>0){
	// 	frontPos = Point(redCircles[0][0],redCircles[0][1]);
	// 	rearPos = Point(greenCircles[0][0],greenCircles[0][1]);
	//  	//cout<<"Car position detected!"<<endl;
	// 	return true;
	// }
	if(redCircles.size()>0){
		frontPos = Point(redCircles[0][0],redCircles[0][1]);
		for(int i=0;i<greenCircles.size();i++){
			if(xyPointDist(greenCircles[i][0],greenCircles[i][1],redCircles[0][0],redCircles[0][1])<CAR_LENGTH){
				rearPos = Point(greenCircles[i][0],greenCircles[i][1]);
				return true;
			}
		}
	}

	cout<<"Can not detect car position."<<endl;
	return false;

	
}

double point_distance(Point& a,Point& b){
	double dx = a.x-b.x;
	double dy = a.y-b.y;
	return sqrt(dx*dx+dy*dy);
}
