#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include "trackInit.h"

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
        imshow("mapProcessed",mapProcessed);

		if(waitKey(1)=='s') break;
	}


    return 0;
}
