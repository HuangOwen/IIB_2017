#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>

using namespace cv;
using namespace std;

int main()
{
     VideoCapture cap(0); //open camera
     if(!cap.isOpened())
        return -1;


    return 0;
}
