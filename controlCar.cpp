#include <iostream>
#include <cmath>
#include <list>
#include <queue>
#include "trackInit.h"
#include "traceCar.h"
#include "controlCar.h"
#include "BLE_PORT.h"
#include "BLE_CAR.h"

#define PI 3.14
#define MIN_TURNPOS 60	// the minimum distance to pop a turning position
#define MIN_DEVIATION_ANGLE 10 // the minimum direction deviation (degree)
#define DIS_TOFAR  40 // the threshold of going too far
#define COMMAND_INTERVAL 150

using namespace std;
using namespace cv;

extern car car_instance;

void go2Target(Point frontPos,Point rearPos,deque<Point>& turningPos){
	Point carCenter = Point((frontPos.x+rearPos.x)/2,(frontPos.y+rearPos.y)/2);


	if(turningPos.size()>0){
		// if(point_distance(turningPos[0],carCenter)<MIN_TURNPOS){
		// if(point_distance(turningPos[0],carCenter)<MIN_TURNPOS && (frontPos.x>turningPos[0].x || frontPos.y>turningPos[0].y)){
		if(point_distance(turningPos[0],frontPos)<MIN_TURNPOS*0.8 || point_distance(turningPos[0],carCenter)<MIN_TURNPOS){
			turningPos.pop_front();
			cout<<"Finished a turning point"<<endl;
			cout<<"Still "<<turningPos.size()<<" points left"<<endl;
		}
	}

	if(turningPos.size()>0){
		Point target = turningPos[0];
		// to avoid divide by zero error
		if((target.x-carCenter.x)==0)
            target.x += 1;
        if((frontPos.x-rearPos.x)==0)
            frontPos.x += 1;
		double slopeTarget_center = (target.y-carCenter.y)/(target.x-carCenter.x);
		if((target.x-frontPos.x)==0)
            target.x += 1;
		double slopeTarget_front = (target.y-frontPos.y)/(target.x-frontPos.x);
		double slopeCar = (frontPos.y-rearPos.y)/(frontPos.x-rearPos.x);
		double theta = (atan(slopeTarget_front)-atan(slopeCar)) * 180 / PI;   //degree unit
		cout<<"Still "<<turningPos.size()<<" points left"<<endl;
		if(abs(theta)>MIN_DEVIATION_ANGLE){
			if(slopeTarget_front>slopeCar || slopeTarget_center>slopeCar){
				cout<<"Turn right"<<endl;
				if(frontPos.x>rearPos.x)
					car_instance.turnr();
				else
					car_instance.turnl();
				_sleep(COMMAND_INTERVAL);
			}
			else{
				cout<<"Turn left"<<endl;
				if(frontPos.x>rearPos.x)
					car_instance.turnl();
				else
					car_instance.turnr();
				_sleep(COMMAND_INTERVAL);
			}
		}
		// else if(carCenter.x > (target.x+DIS_TOFAR)){
		else if(frontPos.x > (target.x+DIS_TOFAR)){
			cout<<"Go back"<<endl;
			if(frontPos.x>rearPos.x)
				car_instance.back();
			else
				car_instance.run();
			_sleep(COMMAND_INTERVAL);
		}
		else{
			cout<<"Go forward"<<endl;
			car_instance.run();
		}
	}
	else{
		car_instance.stop();
		cout<<"STOP"<<endl;
	}
}


