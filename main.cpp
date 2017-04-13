#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include "trackInit.h"

using namespace cv;
using namespace std;

int main()
{
     VideoCapture cap(0); //open camera
     if(!cap.isOpened())
        return -1;

    help();
    genTrack(cap);


    return 0;
}
