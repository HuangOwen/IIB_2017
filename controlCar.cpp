#include <iostream>
#include <cmath>
#include <list>
#include <queue>
#include "trackInit.h"
#include "traceCar.h"
#include "controlCar.h"

#define PI 3.14

using namespace std;
using namespace cv;

void go2Target(Point frontPos,Point rearPos,deque<Point>& turningPos){
	Point carCenter = Point((frontPos.x+rearPos.x)/2,(frontPos.y+rearPos.y)/2);

	if(turningPos.size()>0){
		if(point_distance(turningPos[0],carCenter)<10){
			turningPos.pop_front();
		}
	}

	if(turningPos.size()>0){
		Point target = turningPos[0];
		double slopeTarget = (target.y-carCenter.y)/(target.x-carCenter.x);
		double slopeCar = (frontPos.y-rearPos.y)/(frontPos.x-rearPos.x);
		double theta = (atan(slopeTarget)-atan(slopeCar)) * 180 / PI;   //degree unit
		if(abs(theta)>10){
			if(slopeTarget>slopeCar)
				cout<<"Now turn right"<<endl;
			else
				cout<<"Now turn left"<<endl;
		}
		else
			cout<<"Now go forward"<<endl;

	}
	cout<<"go2Target"<<endl;
}


double point_distance(Point& a,Point& b){
	double dx = a.x-b.x;
	double dy = a.y-b.y;
	return sqrt(dx*dx+dy*dy);
}