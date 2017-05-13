#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include <list>
#include <queue>
#include "trackInit.h"
#include "traceCar.h"
#include "controlCar.h"
#include "BLE_PORT.h"
#include "BLE_CAR.h"



using namespace cv;
using namespace std;

extern Mat perspectiveMatrix;
extern deque<Point> turningPos;
car car_instance;

int main()
{
     VideoCapture cap(0); //open camera
     if(!cap.isOpened()){
        printf("couldn't open camera\n");
        return -1;
     }


    help();

    Mat org;

    while(true){
	    cap >> org;
	    if(org.empty())
            continue;
	    imshow("orginal map",org);
	    if(waitKey(1)=='i'){
	    	destroyWindow("orginal map");
	    	break;
	    }
    }

    genTrack(cap);

    while(true)
	{
	    cap >> org;
		//imshow("orginal video",org);
        Mat mapProcessed;
        warpPerspective(org,mapProcessed,perspectiveMatrix,org.size(),INTER_LINEAR, BORDER_CONSTANT);

        Point frontPos,rearPos;
        Mat mapHSV;
		cvtColor(mapProcessed, mapHSV, CV_BGR2HSV);
        bool carDetected = traceCar(mapHSV,frontPos,rearPos);

        //draw the front pointq
        if(carDetected){
        	circle(mapProcessed, rearPos, 15, Scalar(0,255,0), 3, 8, 0 );
        	circle(mapProcessed, frontPos, 15, Scalar(0,0,255), 3, 8, 0 );
        	//go2Target(frontPos,rearPos,turningPos);
        }

        imshow("mapProcessed",mapProcessed);
        char input = waitKey(1);
        if(input=='s')
            car_instance.stop();
        if(input=='r')
            car_instance.turnr();
        if(input=='l')
            car_instance.turnl();
        if(input=='b')
            car_instance.back();
        if(input=='f')
            car_instance.run();
        if(input=='q')
            break;

	}

    return 0;
}
