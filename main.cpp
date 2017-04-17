#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include "trackInit.h"
#include "traceCar.h"

using namespace cv;
using namespace std;

extern Mat perspectiveMatrix;

int main()
{
     VideoCapture cap(0); //open camera
     if(!cap.isOpened())
        return -1;

    help();
    genTrack(cap);

    for(;;)
	{
		Mat org;
	    cap >> org;
		//imshow("orginal video",org);
        Mat mapProcessed;
        warpPerspective(org,mapProcessed,perspectiveMatrix,org.size(),INTER_LINEAR, BORDER_CONSTANT);
        
        Point frontPos,rearPos;
        Mat mapHSV;
		cvtColor(mapProcessed, mapHSV, CV_BGR2HSV);
        bool carDetected = traceCar(mapHSV,frontPos,rearPos);

        //draw the front point
        if(carDetected){
        	circle(mapProcessed, frontPos, 15, Scalar(0,255,0), 3, 8, 0 );
        	circle(mapProcessed, rearPos, 15, Scalar(0,0,255), 3, 8, 0 );
        }

        imshow("mapProcessed",mapProcessed);
        if(waitKey(1)=='s') break;


		
	}


    return 0;
}
