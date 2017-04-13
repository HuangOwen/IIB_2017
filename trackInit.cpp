#include <iostream>
#include <cmath>
#include "trackInit.h"

using namespace std;
using namespace cv;

void help()
{
    cout << "\tESC - quit the program\n"
         "\tc - stop the tracking\n"
         "\tb - switch to/from backprojection view\n"
         "\tp - pause video\n"
         "\tk - make map\n"
         "\tr - run car\n"
         "\tq - finish map\n"
         "To initialize tracking, select the object with mouse\n";
}

int genTrack(VideoCapture& cap){
    for(;;)
	{
		Mat org;
	    cap >> org;
		imshow("orginal video",org);

		if(waitKey(1)=='s') break;
	}

    cout<<"\nPlease mark the border of the track"<<endl;
}
