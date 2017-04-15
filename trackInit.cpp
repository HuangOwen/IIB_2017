#include <iostream>
#include <cmath>
#include "trackInit.h"

using namespace std;
using namespace cv;


vector<Point> borderPoint; //border selected by user
Mat perspectiveMatrix;  //for changing perspective 
vector<Vec4i> trackLines;  


void help()
{
    cout << "\ts - quit the program\n"
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

    //change perspective
    Mat mapProcessed = changePerspective(orgMap);

    //binarize the map
    Mat mapBinarized = binarize(mapProcessed);
    imshow("mapBinarized",mapBinarized);
    cout<<"The binaried map is displayed. Press any key to continue"<<endl;
    waitKey(0);

    //sharpen the track
    Mat mapSharpend = sharpen(mapBinarized);
    imshow("mapSharpend",mapSharpend);
    cout<<"The sharpened map is displayed. Press any key to continue"<<endl;
    waitKey(0);

    //use Hough line transformation to detect the lines
    HoughLinesP(mapSharpend,trackLines,0.5, CV_PI/90,50,50,50);
    drawHoughLines(mapProcessed);
    imshow("track lines",mapProcessed);
    cout<<"The track lines are displayed. Press any key to continue"<<endl;
    waitKey(0);

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


    perspectiveMatrix = getPerspectiveTransform(srcPoints,dstPoints);
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
            destroyWindow("changePerspective"); //it will expire waitKey(0)
        }
    }
}


Mat binarize(Mat src){
    Mat tmpdst(src.size().height,src.size().width,CV_8U);
    Mat dst(src.size().height,src.size().width,CV_8U);

    cvtColor(src, tmpdst, CV_BGR2GRAY);
    threshold(tmpdst,dst,150, 255, THRESH_BINARY|THRESH_OTSU);

    return dst;
}


Mat sharpen(Mat src){
    Mat tmpdst(src.size().height,src.size().width,CV_8U);
    Mat dst(src.size().height,src.size().width,CV_8U);
    threshold(src, tmpdst, 128, 255, THRESH_BINARY_INV);  //revert the color

    Mat erodeStruct = getStructuringElement(MORPH_RECT,Size(7,7));
    erode(tmpdst,dst,erodeStruct);
    Canny(dst, dst, 50, 150, 3);  //detect the edges

    return dst;
}


void drawHoughLines(Mat& cdst){
    for( size_t i = 0; i < trackLines.size(); i++ ){
        Vec4i l = trackLines[i];
        line(cdst,Point(l[0], l[1]),Point(l[2],l[3]),Scalar(0,255,100),1,CV_AA);
        }
}

