#include <iostream>
#include <cmath>
#include "trackInit.h"

using namespace std;
using namespace cv;

vector<Point> borderPoint;



void help()
{
    cout << "\tESC - quit the program\n"
         "\ts - stop the tracking\n"
         "\tb - switch to/from backprojection view\n"
         "\tp - pause video\n"
         "\tk - make map\n"
         "\tr - run car\n"
         "\tq - finish map\n"
         "To initialize tracking, select the object with mouse\n";
}

int genTrack(VideoCapture& cap){

    Mat orgMap;
    cap >> orgMap;
    Mat finalMap = changePerspective(orgMap);

    for(;;)
	{
		Mat org;
	    cap >> org;
		imshow("orginal video",org);

		if(waitKey(1)=='s') break;
	}
    return 0;
}

Mat changePerspective(Mat org){
    cout<<"\nPlease mark the border of the track."<<endl;
    cout<<"left top point > right top point > right bottom point > left bottom point\n"<<endl;
    namedWindow("changePerspective",CV_WINDOW_AUTOSIZE); //create a window to show the image
    setMouseCallback("changePerspective",onMouse,0);//call onMouse if the user clicks the mouse
    imshow("changePerspective",org);

    waitKey(0);

    Point2f srcPoints[4];
    Point2f dstPoints[4];

    srcPoints[0] = borderPoint[0];
    srcPoints[1] = borderPoint[1];
    srcPoints[2] = borderPoint[2];
    srcPoints[3] = borderPoint[3];

    int height = org.size().height;
    int width = org.size().width;

    dstPoints[0] = Point(0,0);
    dstPoints[1] = Point(width,0);
    dstPoints[2] = Point(width,height);
    dstPoints[3] = Point(0,height);


    Mat perspectiveMatrix = getPerspectiveTransform(srcPoints,dstPoints);
    Mat finalTrack = org.clone();
    warpPerspective(org,finalTrack,perspectiveMatrix,finalTrack.size(),INTER_LINEAR, BORDER_CONSTANT);

    return finalTrack;
}



void onMouse(int event, int x, int y, int flags, void* param){
    Point pt;

    if(event == CV_EVENT_LBUTTONDOWN){
        pt = Point(x,y);
        cout<<"x:"<<x<<" y:"<<y<<endl;
        borderPoint.push_back(pt);

        if(borderPoint.size()>=4){
            cout<<"Selecting border points finished."<<endl;
            cout<<"Please press any key to continue after finishing selecting border points.\n"<<endl;
            destroyWindow("changePerspective");
        }
    }
}
